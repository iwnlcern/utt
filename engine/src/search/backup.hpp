#pragma once

#include "core/types.hpp"
#include "search/tvalue.hpp"

#include <algorithm>
#include <cassert>

namespace uttt {

using Tie = TieState;

struct Aggregates {
  TInterval a{};
  TInterval b{};
  bool has_x = false;
  bool has_o = false;
};

inline void fold_x(Aggregates& aggregates, TInterval child) {
  if (!aggregates.has_x) {
    aggregates.a = child;
    aggregates.has_x = true;
    return;
  }
  aggregates.a.lo = std::min(aggregates.a.lo, child.lo);
  aggregates.a.hi = std::min(aggregates.a.hi, child.hi);
}

inline void fold_o(Aggregates& aggregates, TInterval child) {
  if (!aggregates.has_o) {
    aggregates.b = child;
    aggregates.has_o = true;
    return;
  }
  aggregates.b.lo = std::max(aggregates.b.lo, child.lo);
  aggregates.b.hi = std::max(aggregates.b.hi, child.hi);
}

inline TInterval backup_node(Aggregates aggregates, Tie tie) {
  assert(aggregates.has_x && aggregates.has_o);
  assert(tie == Tie::X || tie == Tie::O);

  if (aggregates.a.hi <= aggregates.b.lo) {
    return f_backup(aggregates.a, aggregates.b);
  }

  const TInterval zugzwang = tie == Tie::X ? aggregates.a : aggregates.b;
  if (aggregates.a.lo > aggregates.b.hi) return zugzwang;

  const TInterval ordered = f_backup(aggregates.a, aggregates.b);
  return {
      std::min(ordered.lo, zugzwang.lo),
      std::max(ordered.hi, zugzwang.hi),
  };
}

inline TInterval critical_r(Aggregates aggregates) {
  assert(aggregates.has_x && aggregates.has_o);

  if (aggregates.a.lo > aggregates.b.hi) return {0.0, 0.0};

  double lo = 0.0;
  if (aggregates.a.hi < aggregates.b.lo) {
    const double numerator = sub_down(aggregates.b.lo, aggregates.a.hi);
    const double denominator = add_up(
        sub_up(1.0, aggregates.a.hi), aggregates.b.lo);
    lo = std::clamp(div_down(numerator, denominator), 0.0, 1.0);
  }

  const double numerator = sub_up(aggregates.b.hi, aggregates.a.lo);
  const double denominator = add_down(
      sub_down(1.0, aggregates.a.lo), aggregates.b.hi);
  const double hi = std::clamp(div_up(numerator, denominator), 0.0, 1.0);
  return {lo, hi};
}

}  // namespace uttt
