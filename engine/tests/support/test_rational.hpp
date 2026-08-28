#pragma once

#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace test_rational_detail {

class BigMagnitude {
 public:
  BigMagnitude() = default;
  explicit BigMagnitude(uint64_t value) {
    if (value != 0) limbs_.push_back(static_cast<uint32_t>(value));
    if (value >> 32) limbs_.push_back(static_cast<uint32_t>(value >> 32));
  }

  [[nodiscard]] bool is_zero() const { return limbs_.empty(); }

  friend bool operator==(const BigMagnitude&, const BigMagnitude&) = default;

  friend int compare(const BigMagnitude& lhs, const BigMagnitude& rhs) {
    if (lhs.limbs_.size() != rhs.limbs_.size()) {
      return lhs.limbs_.size() < rhs.limbs_.size() ? -1 : 1;
    }
    for (std::size_t i = lhs.limbs_.size(); i-- > 0;) {
      if (lhs.limbs_[i] != rhs.limbs_[i]) {
        return lhs.limbs_[i] < rhs.limbs_[i] ? -1 : 1;
      }
    }
    return 0;
  }

  friend BigMagnitude operator+(const BigMagnitude& lhs, const BigMagnitude& rhs) {
    BigMagnitude result;
    const std::size_t size = lhs.limbs_.size() > rhs.limbs_.size()
        ? lhs.limbs_.size()
        : rhs.limbs_.size();
    result.limbs_.resize(size);
    uint64_t carry = 0;
    for (std::size_t i = 0; i < size; ++i) {
      const uint64_t left = i < lhs.limbs_.size() ? lhs.limbs_[i] : 0;
      const uint64_t right = i < rhs.limbs_.size() ? rhs.limbs_[i] : 0;
      const uint64_t sum = left + right + carry;
      result.limbs_[i] = static_cast<uint32_t>(sum);
      carry = sum >> 32;
    }
    if (carry != 0) result.limbs_.push_back(static_cast<uint32_t>(carry));
    return result;
  }

  friend BigMagnitude operator-(const BigMagnitude& lhs, const BigMagnitude& rhs) {
    BigMagnitude result;
    result.limbs_.resize(lhs.limbs_.size());
    uint64_t borrow = 0;
    for (std::size_t i = 0; i < lhs.limbs_.size(); ++i) {
      const uint64_t left = lhs.limbs_[i];
      const uint64_t right = (i < rhs.limbs_.size() ? rhs.limbs_[i] : 0) + borrow;
      result.limbs_[i] = static_cast<uint32_t>(left - right);
      borrow = left < right ? 1 : 0;
    }
    result.normalize();
    return result;
  }

  friend BigMagnitude operator*(const BigMagnitude& lhs, const BigMagnitude& rhs) {
    if (lhs.is_zero() || rhs.is_zero()) return {};
    BigMagnitude result;
    result.limbs_.resize(lhs.limbs_.size() + rhs.limbs_.size());
    for (std::size_t i = 0; i < lhs.limbs_.size(); ++i) {
      uint64_t carry = 0;
      for (std::size_t j = 0; j < rhs.limbs_.size(); ++j) {
        const uint64_t product = uint64_t{lhs.limbs_[i]} * rhs.limbs_[j] +
            result.limbs_[i + j] + carry;
        result.limbs_[i + j] = static_cast<uint32_t>(product);
        carry = product >> 32;
      }
      result.limbs_[i + rhs.limbs_.size()] = static_cast<uint32_t>(carry);
    }
    result.normalize();
    return result;
  }

  [[nodiscard]] BigMagnitude shifted_left(unsigned bits) const {
    if (is_zero()) return {};
    const std::size_t whole_limbs = bits / 32;
    const unsigned partial_bits = bits % 32;
    BigMagnitude result;
    result.limbs_.assign(whole_limbs, 0);
    uint64_t carry = 0;
    for (uint32_t limb : limbs_) {
      const uint64_t shifted = (uint64_t{limb} << partial_bits) | carry;
      result.limbs_.push_back(static_cast<uint32_t>(shifted));
      carry = shifted >> 32;
    }
    if (carry != 0) result.limbs_.push_back(static_cast<uint32_t>(carry));
    return result;
  }

  [[nodiscard]] uint64_t to_uint64() const {
    if (limbs_.size() > 2) throw std::overflow_error("integer result exceeds uint64");
    uint64_t value = limbs_.empty() ? 0 : limbs_[0];
    if (limbs_.size() == 2) value |= uint64_t{limbs_[1]} << 32;
    return value;
  }

  friend std::pair<BigMagnitude, BigMagnitude> divmod(
      const BigMagnitude& numerator, const BigMagnitude& denominator) {
    if (denominator.is_zero()) throw std::domain_error("division by zero");
    BigMagnitude quotient;
    BigMagnitude remainder;
    for (std::size_t bit = numerator.bit_count(); bit-- > 0;) {
      remainder.shift_left_one();
      if (numerator.bit_at(bit)) remainder.add_one();
      if (compare(remainder, denominator) >= 0) {
        remainder = remainder - denominator;
        quotient.set_bit(bit);
      }
    }
    return {quotient, remainder};
  }

 private:
  std::vector<uint32_t> limbs_;

  void normalize() {
    while (!limbs_.empty() && limbs_.back() == 0) limbs_.pop_back();
  }

  [[nodiscard]] std::size_t bit_count() const {
    if (is_zero()) return 0;
    return (limbs_.size() - 1) * 32 +
        (32u - static_cast<unsigned>(std::countl_zero(limbs_.back())));
  }

  [[nodiscard]] bool bit_at(std::size_t bit) const {
    return ((limbs_[bit / 32] >> (bit % 32)) & 1u) != 0;
  }

  void set_bit(std::size_t bit) {
    if (limbs_.size() <= bit / 32) limbs_.resize(bit / 32 + 1);
    limbs_[bit / 32] |= uint32_t{1} << (bit % 32);
  }

  void shift_left_one() {
    uint64_t carry = 0;
    for (uint32_t& limb : limbs_) {
      const uint64_t shifted = (uint64_t{limb} << 1) | carry;
      limb = static_cast<uint32_t>(shifted);
      carry = shifted >> 32;
    }
    if (carry != 0) limbs_.push_back(static_cast<uint32_t>(carry));
  }

  void add_one() {
    uint64_t carry = 1;
    for (uint32_t& limb : limbs_) {
      const uint64_t sum = uint64_t{limb} + carry;
      limb = static_cast<uint32_t>(sum);
      carry = sum >> 32;
      if (carry == 0) return;
    }
    if (carry != 0) limbs_.push_back(1);
  }
};

inline BigMagnitude gcd(BigMagnitude lhs, BigMagnitude rhs) {
  while (!rhs.is_zero()) {
    auto [ignored, remainder] = divmod(lhs, rhs);
    (void)ignored;
    lhs = std::move(rhs);
    rhs = std::move(remainder);
  }
  return lhs;
}

inline BigMagnitude magnitude_of(int64_t value) {
  const uint64_t magnitude = value < 0
      ? static_cast<uint64_t>(-(value + 1)) + 1
      : static_cast<uint64_t>(value);
  return BigMagnitude{magnitude};
}

}  // namespace test_rational_detail

struct TestRational {
  explicit TestRational(int64_t numerator = 0, int64_t denominator = 1)
      : negative_((numerator < 0) != (denominator < 0)),
        numerator_(test_rational_detail::magnitude_of(numerator)),
        denominator_(test_rational_detail::magnitude_of(denominator)) {
    if (denominator == 0) throw std::invalid_argument("rational denominator is zero");
    normalize();
  }

  static TestRational from_double(double value) {
    if (!std::isfinite(value)) throw std::invalid_argument("non-finite rational input");
    const uint64_t bits = std::bit_cast<uint64_t>(value);
    const bool negative = (bits >> 63) != 0;
    const uint64_t exponent_bits = (bits >> 52) & 0x7ffu;
    const uint64_t fraction = bits & ((uint64_t{1} << 52) - 1);
    if (exponent_bits == 0 && fraction == 0) return TestRational{};

    const uint64_t mantissa = exponent_bits == 0
        ? fraction
        : fraction | (uint64_t{1} << 52);
    const int exponent = exponent_bits == 0
        ? -1074
        : static_cast<int>(exponent_bits) - 1023 - 52;
    BigMagnitude numerator{mantissa};
    BigMagnitude denominator{1};
    if (exponent >= 0) {
      numerator = numerator.shifted_left(static_cast<unsigned>(exponent));
    } else {
      denominator = denominator.shifted_left(static_cast<unsigned>(-exponent));
    }
    return TestRational(negative, std::move(numerator), std::move(denominator));
  }

  [[nodiscard]] bool inside(double lo, double hi) const {
    return from_double(lo) <= *this && *this <= from_double(hi);
  }

  friend TestRational operator+(const TestRational& lhs, const TestRational& rhs) {
    const BigMagnitude left = lhs.numerator_ * rhs.denominator_;
    const BigMagnitude right = rhs.numerator_ * lhs.denominator_;
    const BigMagnitude denominator = lhs.denominator_ * rhs.denominator_;
    if (lhs.negative_ == rhs.negative_) {
      return TestRational(lhs.negative_, left + right, denominator);
    }
    const int order = compare(left, right);
    if (order == 0) return TestRational{};
    return order > 0
        ? TestRational(lhs.negative_, left - right, denominator)
        : TestRational(rhs.negative_, right - left, denominator);
  }

  friend TestRational operator-(const TestRational& lhs, const TestRational& rhs) {
    TestRational negated = rhs;
    if (!negated.numerator_.is_zero()) negated.negative_ = !negated.negative_;
    return lhs + negated;
  }

  friend TestRational operator*(const TestRational& lhs, const TestRational& rhs) {
    return TestRational(lhs.negative_ != rhs.negative_,
                        lhs.numerator_ * rhs.numerator_,
                        lhs.denominator_ * rhs.denominator_);
  }

  friend TestRational operator/(const TestRational& lhs, const TestRational& rhs) {
    if (rhs.numerator_.is_zero()) throw std::domain_error("rational division by zero");
    return TestRational(lhs.negative_ != rhs.negative_,
                        lhs.numerator_ * rhs.denominator_,
                        lhs.denominator_ * rhs.numerator_);
  }

  friend bool operator==(const TestRational& lhs, const TestRational& rhs) {
    return lhs.negative_ == rhs.negative_ && lhs.numerator_ == rhs.numerator_ &&
        lhs.denominator_ == rhs.denominator_;
  }

  friend bool operator<(const TestRational& lhs, const TestRational& rhs) {
    if (lhs.negative_ != rhs.negative_) return lhs.negative_;
    const int order = compare(lhs.numerator_ * rhs.denominator_,
                              rhs.numerator_ * lhs.denominator_);
    return lhs.negative_ ? order > 0 : order < 0;
  }

  friend bool operator<=(const TestRational& lhs, const TestRational& rhs) {
    return !(rhs < lhs);
  }
  friend bool operator>(const TestRational& lhs, const TestRational& rhs) {
    return rhs < lhs;
  }
  friend bool operator>=(const TestRational& lhs, const TestRational& rhs) {
    return !(lhs < rhs);
  }

  friend int64_t half_up_int(const TestRational& value) {
    const BigMagnitude twice_numerator = value.numerator_.shifted_left(1);
    const BigMagnitude twice_denominator = value.denominator_.shifted_left(1);
    bool result_negative = false;
    BigMagnitude adjusted;
    if (!value.negative_) {
      adjusted = twice_numerator + value.denominator_;
    } else {
      const int order = compare(twice_numerator, value.denominator_);
      if (order == 0) return 0;
      result_negative = order > 0;
      adjusted = order > 0
          ? twice_numerator - value.denominator_
          : value.denominator_ - twice_numerator;
    }

    auto [quotient, remainder] = divmod(adjusted, twice_denominator);
    if (result_negative && !remainder.is_zero()) quotient = quotient + BigMagnitude{1};
    const uint64_t magnitude = quotient.to_uint64();
    if (!result_negative) {
      if (magnitude > static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
        throw std::overflow_error("rounded rational exceeds int64");
      }
      return static_cast<int64_t>(magnitude);
    }
    const uint64_t min_magnitude = uint64_t{1} << 63;
    if (magnitude > min_magnitude) throw std::overflow_error("rounded rational exceeds int64");
    if (magnitude == min_magnitude) return std::numeric_limits<int64_t>::min();
    return -static_cast<int64_t>(magnitude);
  }

 private:
  using BigMagnitude = test_rational_detail::BigMagnitude;

  bool negative_ = false;
  BigMagnitude numerator_;
  BigMagnitude denominator_{1};

  TestRational(bool negative, BigMagnitude numerator, BigMagnitude denominator)
      : negative_(negative),
        numerator_(std::move(numerator)),
        denominator_(std::move(denominator)) {
    normalize();
  }

  void normalize() {
    if (denominator_.is_zero()) throw std::invalid_argument("rational denominator is zero");
    if (numerator_.is_zero()) {
      negative_ = false;
      denominator_ = BigMagnitude{1};
      return;
    }
    const BigMagnitude divisor = test_rational_detail::gcd(numerator_, denominator_);
    numerator_ = divmod(numerator_, divisor).first;
    denominator_ = divmod(denominator_, divisor).first;
  }
};
