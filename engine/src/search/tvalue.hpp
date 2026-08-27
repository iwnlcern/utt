#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

namespace uttt {

struct TInterval {
  double lo;
  double hi;
};

inline double add_down(double lhs, double rhs) {
  return std::nextafter(lhs + rhs, -std::numeric_limits<double>::infinity());
}

inline double add_up(double lhs, double rhs) {
  return std::nextafter(lhs + rhs, std::numeric_limits<double>::infinity());
}

inline double sub_down(double lhs, double rhs) {
  return std::nextafter(lhs - rhs, -std::numeric_limits<double>::infinity());
}

inline double sub_up(double lhs, double rhs) {
  return std::nextafter(lhs - rhs, std::numeric_limits<double>::infinity());
}

inline double mul_down(double lhs, double rhs) {
  return std::nextafter(lhs * rhs, -std::numeric_limits<double>::infinity());
}

inline double mul_up(double lhs, double rhs) {
  return std::nextafter(lhs * rhs, std::numeric_limits<double>::infinity());
}

inline double div_down(double lhs, double rhs) {
  return std::nextafter(lhs / rhs, -std::numeric_limits<double>::infinity());
}

inline double div_up(double lhs, double rhs) {
  return std::nextafter(lhs / rhs, std::numeric_limits<double>::infinity());
}

inline TInterval f_backup(TInterval a, TInterval b) {
  const double lo_denominator = add_up(sub_up(1.0, a.lo), b.lo);
  const double hi_denominator = add_down(sub_down(1.0, a.hi), b.hi);
  return {
      std::clamp(div_down(b.lo, lo_denominator), 0.0, 1.0),
      std::clamp(div_up(b.hi, hi_denominator), 0.0, 1.0),
  };
}

inline bool contains(TInterval interval, double value) {
  return interval.lo <= value && value <= interval.hi;
}

inline double width(TInterval interval) {
  return interval.hi - interval.lo;
}

}  // namespace uttt
