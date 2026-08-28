#include "root/bid_matrix.hpp"

#include <algorithm>
#include <bit>
#include <cmath>

namespace uttt {

std::vector<int64_t> candidate_bids(int64_t stack, int64_t k_star) {
  if (stack < 0 || stack > std::numeric_limits<uint32_t>::max())
    throw std::invalid_argument("candidate-bid stack is out of range");
  std::vector<int64_t> result = {
      0, stack, stack / 4, stack / 2, (3 * stack) / 4,
  };
  for (int offset = -2; offset <= 2; ++offset) {
    if ((offset < 0 && k_star < -offset) ||
        (offset > 0 && k_star > std::numeric_limits<int64_t>::max() - offset))
      continue;
    const int64_t bid = k_star + offset;
    if (0 <= bid && bid <= stack)
      result.push_back(bid);
  }
  std::sort(result.begin(), result.end());
  result.erase(std::unique(result.begin(), result.end()), result.end());
  return result;
}

int64_t exact_half_up_product(double fraction, int64_t scale) {
  if (!std::isfinite(fraction) || fraction < 0.0 || fraction > 1.0 ||
      scale < 0 || scale > std::numeric_limits<uint32_t>::max()) {
    throw std::invalid_argument("half-up product inputs are out of range");
  }
  if (fraction == 0.0 || scale == 0)
    return 0;
  if (fraction == 1.0)
    return scale;
  const auto decomposed = p2_detail::decompose_unit_endpoint(fraction);
  const p2_detail::Wide numerator =
      static_cast<p2_detail::Wide>(decomposed.mantissa) *
      static_cast<uint64_t>(scale);
  if (decomposed.exponent >= 0) {
    return static_cast<int64_t>(numerator << decomposed.exponent);
  }
  const unsigned shift = static_cast<unsigned>(-decomposed.exponent);
  if (shift >= 128)
    return 0;
  const p2_detail::Wide quotient = numerator >> shift;
  const p2_detail::Wide remainder = numerator - (quotient << shift);
  const p2_detail::Wide half = p2_detail::Wide{1} << (shift - 1);
  return static_cast<int64_t>(quotient + (remainder >= half));
}

} // namespace uttt
