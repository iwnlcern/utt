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
  const double hi = hi_denominator <= 0.0
      ? 1.0
      : std::clamp(div_up(b.hi, hi_denominator), 0.0, 1.0);
  return {
      std::clamp(div_down(b.lo, lo_denominator), 0.0, 1.0),
      hi,
  };
}

inline double x_preimage_down(double t, double b) {
  if (t == 0.0) return 0.0;
  return std::clamp(
      sub_down(1.0, div_up(mul_up(b, sub_up(1.0, t)), t)),
      0.0,
      1.0);
}

inline double x_preimage_up(double t, double b) {
  if (t == 0.0) return 1.0;
  return std::clamp(
      sub_up(1.0, div_down(mul_down(b, sub_down(1.0, t)), t)),
      0.0,
      1.0);
}

inline TInterval x_preimage(TInterval t, TInterval b) {
  return {
      x_preimage_down(t.lo, b.hi),
      x_preimage_up(t.hi, b.lo),
  };
}

inline double o_preimage_down(double t, double a) {
  if (t == 1.0) return 0.0;
  return std::clamp(
      div_down(mul_down(t, sub_down(1.0, a)), sub_up(1.0, t)),
      0.0,
      1.0);
}

inline double o_preimage_up(double t, double a) {
  if (t == 1.0) return 1.0;
  return std::clamp(
      div_up(mul_up(t, sub_up(1.0, a)), sub_down(1.0, t)),
      0.0,
      1.0);
}

inline TInterval o_preimage(TInterval t, TInterval a) {
  return {
      o_preimage_down(t.lo, a.hi),
      o_preimage_up(t.hi, a.lo),
  };
}

inline bool contains(TInterval interval, double value) {
  return interval.lo <= value && value <= interval.hi;
}

inline double width(TInterval interval) {
  return interval.hi - interval.lo;
}

}  // namespace uttt
