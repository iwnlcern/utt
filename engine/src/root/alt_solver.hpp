#pragma once

#include "search/search.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace uttt {

struct AltResult {
  double value = 0.0;
  Quality quality = Quality::Estimate;
  bool complete = false;
};

struct AltLimits {
  int max_depth = 0;
  uint64_t node_cap = std::numeric_limits<uint64_t>::max();
  uint64_t stop_check_nodes = 256;
  std::function<bool()> stop_requested{};
};

template <GameModel M> class AltSolver {
public:
  using State = typename M::State;
  using EvalFn = std::function<double(const State &)>;
  using HashFn = std::function<std::size_t(const PosId &, Tie, int)>;

  explicit AltSolver(EvalFn evaluate = {}, HashFn hash = {})
      : evaluate_(std::move(evaluate)), hash_(std::move(hash)) {}

  AltResult solve(const State &state, Tie h, AltLimits limits) {
    if ((h != Tie::X && h != Tie::O) || limits.max_depth < 0)
      throw std::invalid_argument("alternation solver inputs are out of range");
    nodes_ = 0;
    node_cap_ = limits.node_cap;
    stop_check_nodes_ = std::max<uint64_t>(limits.stop_check_nodes, 1);
    stop_requested_ = std::move(limits.stop_requested);
    cancelled_ = false;
    if (poll_stop())
      return {};
    const PosId id = M::pos_id(state);
    if (const auto cached = lookup(id, h, limits.max_depth))
      return *cached;
    ++unique_root_searches_;
    return dfs(state, h, limits.max_depth);
  }

  uint64_t unique_root_searches() const { return unique_root_searches_; }
  std::size_t memo_entries() const { return memo_entries_; }
  bool was_cancelled() const { return cancelled_; }

private:
  struct Entry {
    PosId id;
    Tie h;
    int remaining_depth;
    AltResult result;
  };

  EvalFn evaluate_;
  HashFn hash_;
  std::unordered_map<std::size_t, std::vector<Entry>> memo_;
  std::size_t memo_entries_ = 0;
  uint64_t unique_root_searches_ = 0;
  uint64_t nodes_ = 0;
  uint64_t node_cap_ = std::numeric_limits<uint64_t>::max();
  uint64_t stop_check_nodes_ = 256;
  std::function<bool()> stop_requested_{};
  bool cancelled_ = false;

  bool poll_stop() {
    if (cancelled_)
      return true;
    if (stop_requested_ && stop_requested_()) {
      cancelled_ = true;
      return true;
    }
    return false;
  }

  static void mix(std::size_t &seed, uint64_t value) {
    seed ^= static_cast<std::size_t>(value) + 0x9e3779b97f4a7c15ULL +
            (seed << 6) + (seed >> 2);
  }

  static std::size_t default_hash(const PosId &id, Tie h, int remaining_depth) {
    std::size_t seed = 0;
    for (uint16_t value : id.x)
      mix(seed, value);
    for (uint16_t value : id.o)
      mix(seed, value);
    mix(seed, static_cast<uint8_t>(id.forced));
    mix(seed, static_cast<uint8_t>(id.tie));
    mix(seed, static_cast<uint8_t>(h));
    mix(seed, static_cast<uint32_t>(remaining_depth));
    return seed;
  }

  std::size_t bucket(const PosId &id, Tie h, int remaining_depth) const {
    return hash_ ? hash_(id, h, remaining_depth)
                 : default_hash(id, h, remaining_depth);
  }

  std::optional<AltResult> lookup(const PosId &id, Tie h,
                                  int remaining_depth) const {
    const auto found = memo_.find(bucket(id, h, remaining_depth));
    if (found == memo_.end())
      return std::nullopt;
    for (const Entry &entry : found->second) {
      if (entry.id == id && entry.h == h &&
          entry.remaining_depth == remaining_depth)
        return entry.result;
    }
    return std::nullopt;
  }

  void store(const PosId &id, Tie h, int remaining_depth, AltResult result) {
    memo_[bucket(id, h, remaining_depth)].push_back(
        {id, h, remaining_depth, result});
    ++memo_entries_;
  }

  double horizon(const State &state) const {
    double estimate = 0.5;
    if (evaluate_) {
      estimate = evaluate_(state);
    } else if constexpr (requires { M::estimate(state); }) {
      estimate = M::estimate(state);
    } else if constexpr (std::same_as<State, Position>) {
      estimate = eval_estimate(state);
    }
    return std::clamp(1.0 - 2.0 * estimate, -1.0, 1.0);
  }

  AltResult dfs(const State &state, Tie h, int remaining_depth) {
    const PosId id = M::pos_id(state);
    if (const auto cached = lookup(id, h, remaining_depth))
      return *cached;
    if ((nodes_ % stop_check_nodes_ == 0 && poll_stop()) || nodes_ >= node_cap_)
      return {};
    ++nodes_;

    AltResult result;
    switch (M::terminal(state)) {
    case TerminalKind::MacroWinX:
      result = {1.0, Quality::Exact, true};
      break;
    case TerminalKind::MacroWinO:
      result = {-1.0, Quality::Exact, true};
      break;
    case TerminalKind::AllClosed:
      result = {0.0, Quality::Exact, true};
      break;
    case TerminalKind::None:
      if (remaining_depth == 0) {
        result = {horizon(state), Quality::Estimate, true};
      } else {
        const auto children =
            h == Tie::X ? M::children_x(state) : M::children_o(state);
        if (children.empty())
          throw std::logic_error(
              "nonterminal alternation state has no mover children");
        result = {h == Tie::X ? -1.0 : 1.0, Quality::Exact, true};
        const Tie successor = h == Tie::X ? Tie::O : Tie::X;
        for (const auto &child : children) {
          const AltResult child_result =
              dfs(child.state, successor, remaining_depth - 1);
          if (!child_result.complete)
            return {};
          result.value = h == Tie::X
                             ? std::max(result.value, child_result.value)
                             : std::min(result.value, child_result.value);
          if (child_result.quality == Quality::Estimate)
            result.quality = Quality::Estimate;
        }
      }
      break;
    }
    store(id, h, remaining_depth, result);
    return result;
  }
};

} // namespace uttt
