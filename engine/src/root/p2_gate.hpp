#pragma once

#include "search/tvalue.hpp"

#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace uttt {

enum RootClass { XForced, OForced, InBand };

namespace p2_detail {

using Wide = unsigned __int128;

struct Binary64 {
  uint64_t mantissa;
  int exponent;
};

inline Binary64 decompose_unit_endpoint(double value) {
  const uint64_t bits = std::bit_cast<uint64_t>(value);
  const uint64_t exponent_bits = (bits >> 52) & 0x7ffu;
  const uint64_t fraction = bits & ((uint64_t{1} << 52) - 1);
  return {
      exponent_bits == 0 ? fraction : fraction | (uint64_t{1} << 52),
      exponent_bits == 0 ? -1074 : static_cast<int>(exponent_bits) - 1023 - 52,
  };
}

inline bool shift_overflows_wide(uint64_t magnitude, unsigned shift) {
  if (magnitude == 0)
    return false;
  if (shift >= 128)
    return true;
  return std::bit_width(magnitude) + shift > 128;
}

inline Wide shifted_wide(uint64_t magnitude, unsigned shift) {
  if (magnitude == 0)
    return 0;
  return static_cast<Wide>(magnitude) << shift;
}

inline Wide scaled_mantissa(Binary64 endpoint, uint64_t scale) {
  return static_cast<Wide>(endpoint.mantissa) * static_cast<Wide>(scale);
}

inline uint64_t ceil_scaled_endpoint(Binary64 endpoint, uint64_t scale) {
  const Wide numerator = scaled_mantissa(endpoint, scale);
  if (numerator == 0)
    return 0;
  if (endpoint.exponent >= 0) {
    const unsigned shift = static_cast<unsigned>(endpoint.exponent);
    if (shift >= 128 ||
        numerator >
            (static_cast<Wide>(std::numeric_limits<uint64_t>::max()) >> shift))
      throw std::overflow_error("exact endpoint ceiling exceeds uint64_t");
    return static_cast<uint64_t>(numerator << shift);
  }
  const unsigned shift = static_cast<unsigned>(-endpoint.exponent);
  if (shift >= 128)
    return 1;
  const Wide quotient = numerator >> shift;
  const Wide remainder = numerator - (quotient << shift);
  const Wide result = quotient + (remainder != 0 ? 1 : 0);
  if (result > std::numeric_limits<uint64_t>::max())
    throw std::overflow_error("exact endpoint ceiling exceeds uint64_t");
  return static_cast<uint64_t>(result);
}

inline bool x_margin_exceeds_endpoint(int64_t margin, Binary64 endpoint,
                                      uint64_t scale) {
  if (margin <= 0)
    return false;
  const uint64_t magnitude = static_cast<uint64_t>(margin);
  const unsigned shift = static_cast<unsigned>(-endpoint.exponent);
  if (shift_overflows_wide(magnitude, shift))
    return true;
  return shifted_wide(magnitude, shift) > scaled_mantissa(endpoint, scale);
}

inline bool endpoint_exceeds_o_margin(Binary64 endpoint, uint64_t scale,
                                      uint64_t margin) {
  const unsigned shift = static_cast<unsigned>(-endpoint.exponent);
  if (shift_overflows_wide(margin, shift))
    return false;
  return scaled_mantissa(endpoint, scale) > shifted_wide(margin, shift);
}

inline void validate_p2_inputs(TInterval t, int64_t bx, int64_t total,
                               int empties) {
  constexpr int64_t kMaxTotal =
      static_cast<int64_t>(std::numeric_limits<uint32_t>::max());
  if (!std::isfinite(t.lo) || !std::isfinite(t.hi) || t.lo < 0.0 ||
      t.hi > 1.0 || t.lo > t.hi) {
    throw std::invalid_argument(
        "P2 interval must be finite, ordered, and in [0,1]");
  }
  if (total < 0 || total > kMaxTotal || bx < 0 || bx > total) {
    throw std::invalid_argument("P2 budgets must satisfy 0 <= bx <= M < 2^32");
  }
  if (empties < 0 || empties > 81) {
    throw std::invalid_argument("P2 empty-cell count must be in [0,81]");
  }
}

} // namespace p2_detail

inline uint64_t ceil_exact(double endpoint, uint64_t scale) {
  if (!std::isfinite(endpoint) || endpoint < 0.0 || endpoint > 1.0 ||
      scale > std::numeric_limits<uint32_t>::max())
    throw std::invalid_argument(
        "exact endpoint ceiling inputs must be t in [0,1], M < 2^32");
  return p2_detail::ceil_scaled_endpoint(
      p2_detail::decompose_unit_endpoint(endpoint), scale);
}

inline RootClass p2_classify(TInterval t, int64_t bx, int64_t total,
                             int empties) {
  p2_detail::validate_p2_inputs(t, bx, total, empties);

  const int64_t x_margin = bx - empties;
  const bool x_forced =
      t.hi == 0.0   ? bx > empties
      : t.hi == 1.0 ? x_margin > total
                    : p2_detail::x_margin_exceeds_endpoint(
                          x_margin, p2_detail::decompose_unit_endpoint(t.hi),
                          static_cast<uint64_t>(total));
  if (x_forced)
    return XForced;

  const bool o_forced = t.lo == 0.0 ? false
                        : t.lo == 1.0
                            ? total - bx > empties
                            : p2_detail::endpoint_exceeds_o_margin(
                                  p2_detail::decompose_unit_endpoint(t.lo),
                                  static_cast<uint64_t>(total),
                                  static_cast<uint64_t>(bx + empties));
  if (o_forced)
    return OForced;
  return InBand;
}

} // namespace uttt
