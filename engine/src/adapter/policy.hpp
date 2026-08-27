#pragma once

#include "adapter/wire.hpp"
#include "core/clock.hpp"
#include "root/alt_solver.hpp"
#include "root/bid_matrix.hpp"
#include "root/rmplus.hpp"
#include "search/backup.hpp"
#include "search/search.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace uttt {

struct Policy {
  virtual wire::TurnReply choose(const wire::TurnRequest &, Clock &) = 0;
  virtual ~Policy() = default;
};

struct PolicyDiagnostics {
  bool p2_checked = false;
  RootClass root_class = InBand;
  bool root_search_cancelled = false;
  bool child_search_cancelled = false;
  bool matrix_bypassed = false;
  bool matrix_constructed = false;
  bool matrix_complete = false;
  bool matrix_solved = false;
  bool matrix_action_published = false;
  uint64_t matrix_entries = 0;
  int rm_iterations = 0;
};

namespace policy_detail {

inline uint8_t flat(Move move) {
  return static_cast<uint8_t>(9 * move.board + move.cell);
}

inline Move unflatten(uint8_t move) {
  return {static_cast<uint8_t>(move / 9), static_cast<uint8_t>(move % 9)};
}

inline const char *quality_name(Quality quality) {
  switch (quality) {
  case Quality::Exact:
    return "exact";
  case Quality::Bound:
    return "bound";
  case Quality::Estimate:
    return "estimate";
  }
  return "estimate";
}

inline uint8_t legal_preferred(const wire::TurnRequest &request,
                               uint8_t preferred) {
  const auto found =
      std::find_if(request.legal.begin(), request.legal.end(),
                   [preferred](Move move) { return flat(move) == preferred; });
  return found == request.legal.end() ? flat(request.legal.front()) : preferred;
}

struct Decision {
  int64_t bid = 0;
  uint8_t move = 0;
  SearchResult searched{};
};

} // namespace policy_detail

struct EnginePolicy final : Policy {
  const PolicyDiagnostics &last_diagnostics() const { return diagnostics_; }

  wire::TurnReply choose(const wire::TurnRequest &request,
                         Clock &clock) override {
    assert(!request.legal.empty());
    diagnostics_ = {};
    const int64_t start = clock.now_ms();
    const RequestDeadlines deadlines =
        request_deadlines(start, request.time_ms);
    const uint8_t first = policy_detail::flat(request.legal.front());
    SearchResult fallback{{0.0, 1.0},        first, first,
                          Quality::Estimate, 0,     false};
    policy_detail::Decision decision{0, first, fallback};
    const Tie action_tie = request.pos.tie == Tie::NullFirstMove
                               ? (request.ctx.seat == Seat::X ? Tie::X : Tie::O)
                               : request.pos.tie;

    Search<UtttModel> root_search(16);
    const SearchResult published = run_root_stages<SearchResult>(
        fallback, 1, 4, clock, deadlines,
        [&](int depth, int64_t hard) -> std::optional<SearchResult> {
          const SearchResult result = root_search.solve(
              request.pos, request.pos.tie,
              Limits{depth, 25'000, true, true, 12, 64,
                     [&] { return clock.now_ms() >= deadlines.search_stop; }});
          diagnostics_.root_search_cancelled =
              diagnostics_.root_search_cancelled || root_search.was_cancelled();
          if (!result.complete || clock.now_ms() >= hard)
            return std::nullopt;
          return result;
        },
        [&](SearchResult staged, int64_t hard) {
          decision.searched = staged;
          const int64_t bx = request.ctx.budget_x;
          const int64_t bo = request.ctx.budget_o;
          const RootClass root_class = p2_classify(
              staged.t, bx, bx + bo, UtttModel::empties(request.pos));
          diagnostics_.p2_checked = true;
          diagnostics_.root_class = root_class;
          if (root_class != InBand || clock.now_ms() >= hard) {
            diagnostics_.matrix_bypassed = root_class != InBand;
            const bool own_forced =
                (root_class == XForced && request.ctx.seat == Seat::X) ||
                (root_class == OForced && request.ctx.seat == Seat::O);
            const int64_t own_stack = request.ctx.seat == Seat::X ? bx : bo;
            decision.bid =
                own_forced ? std::min<int64_t>(
                                 own_stack, UtttModel::empties(request.pos) + 1)
                           : 0;
            const uint8_t preferred =
                request.ctx.seat == Seat::X ? staged.best_x : staged.best_o;
            decision.move = policy_detail::legal_preferred(request, preferred);
            return staged;
          }

          const int child_depth = std::max<int>(1, staged.depth - 1);
          Search<UtttModel> child_search(16);
          const auto solve_child = [&](const Position &child, Tie h2) {
            SearchResult result = child_search.solve(
                child, h2, Limits{child_depth, 12'000, true, true, 12, 64, [&] {
                                    return clock.now_ms() >= hard;
                                  }});
            diagnostics_.child_search_cancelled =
                diagnostics_.child_search_cancelled ||
                child_search.was_cancelled();
            return result;
          };

          Aggregates aggregates{};
          for (const auto &child : UtttModel::children_x(request.pos))
            fold_x(aggregates, solve_child(child.state, Tie::O).t);
          for (const auto &child : UtttModel::children_o(request.pos))
            fold_o(aggregates, solve_child(child.state, Tie::X).t);
          const uint8_t best_x =
              policy_detail::legal_preferred(request, staged.best_x);
          const uint8_t best_o =
              policy_detail::legal_preferred(request, staged.best_o);
          const Anchors anchors = production_anchors(aggregates.a, aggregates.b,
                                                     bx + bo, best_x, best_o);

          struct CachedChild {
            Position state;
            Tie tie;
            SearchResult result;
          };
          std::vector<CachedChild> cache;
          AltSolver<UtttModel> alternate;
          PayoffFn<Position> payoff =
              [&](Position child, Tie h2, int64_t bx2,
                  int64_t bo2) -> std::optional<PayoffResult> {
            if (UtttModel::terminal(child) != TerminalKind::None)
              return production_payoff<UtttModel>(child, h2, bx2, bo2, {});
            if (bx2 + bo2 == 0) {
              const AltResult result = alternate.solve(
                  child, h2, AltLimits{child_depth, 12'000, 64, [&] {
                                         return clock.now_ms() >= hard;
                                       }});
              diagnostics_.child_search_cancelled =
                  diagnostics_.child_search_cancelled ||
                  alternate.was_cancelled();
              if (!result.complete)
                return std::nullopt;
              return PayoffResult{result.value,
                                  result.quality == Quality::Exact};
            }
            auto found = std::find_if(
                cache.begin(), cache.end(), [&](const CachedChild &entry) {
                  return entry.tie == h2 && entry.state.identity_equal(child);
                });
            if (found == cache.end()) {
              cache.push_back({child, h2, solve_child(child, h2)});
              found = std::prev(cache.end());
            }
            if (!found->result.complete)
              return std::nullopt;
            return production_payoff<UtttModel>(child, h2, bx2, bo2,
                                                found->result);
          };
          diagnostics_.matrix_constructed = true;
          const auto matrix = build_bid_matrix<UtttModel>(
              request.pos, action_tie, bx, bo, anchors, std::move(payoff),
              [&] { return clock.now_ms() >= hard; });
          diagnostics_.matrix_complete = matrix.complete;
          diagnostics_.matrix_entries = matrix.entries_evaluated;
          if (!matrix.complete)
            return staged;
          const RMPlusResult solution = solve_rmplus(
              matrix.payoffs, 10'000, [&] { return clock.now_ms() >= hard; });
          diagnostics_.rm_iterations = solution.iterations;
          if (solution.iterations == 0 || clock.now_ms() >= hard)
            return staged;
          diagnostics_.matrix_solved = true;
          const std::size_t selected =
              select_root_action(matrix, solution, request.ctx.seat);
          const auto &actions = request.ctx.seat == Seat::X
                                    ? matrix.row_actions
                                    : matrix.column_actions;
          decision.bid = actions[selected].bid;
          decision.move = actions[selected].move;
          diagnostics_.matrix_action_published = true;
          staged.quality = matrix_quality(staged.quality, matrix.all_exact);
          decision.searched = staged;
          return staged;
        });
    decision.searched = published;
    const Move move = policy_detail::unflatten(decision.move);
    assert(std::find(request.legal.begin(), request.legal.end(), move) !=
           request.legal.end());
    return {
        request.request_id,
        decision.bid,
        move,
        {policy_detail::quality_name(decision.searched.quality),
         decision.searched.t.lo, decision.searched.t.hi,
         decision.searched.depth, decision.searched.complete},
    };
  }

private:
  PolicyDiagnostics diagnostics_{};
};

using PlaceholderPolicy = EnginePolicy;

} // namespace uttt
