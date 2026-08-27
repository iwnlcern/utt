#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>

namespace uttt {

struct Clock {
  virtual int64_t now_ms() = 0;
  virtual ~Clock() = default;
};

struct FakeClock final : Clock {
  int64_t t = 0;
  int64_t now_ms() override { return t; }
};

struct SteadyClock final : Clock {
  int64_t now_ms() override {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
  }
};

struct RequestDeadlines {
  int64_t soft;
  int64_t hard;
  int64_t reserve;
  int64_t search_stop;
};

inline int64_t scaled_request_ms(int64_t time_ms, int64_t numerator) {
  if (time_ms < 0 || numerator < 0 || numerator > 30)
    throw std::invalid_argument("request deadline input is out of range");
  return (time_ms / 30) * numerator + ((time_ms % 30) * numerator) / 30;
}

inline int64_t saturating_deadline(int64_t start, int64_t delta) {
  if (delta > 0 && start > std::numeric_limits<int64_t>::max() - delta)
    return std::numeric_limits<int64_t>::max();
  return start + delta;
}

inline RequestDeadlines request_deadlines(int64_t start, int64_t time_ms) {
  const int64_t soft_delta = scaled_request_ms(time_ms, 27);
  const int64_t hard_delta = scaled_request_ms(time_ms, 29);
  const int64_t reserve = std::min<int64_t>(500, time_ms / 6);
  return {
      saturating_deadline(start, soft_delta),
      saturating_deadline(start, hard_delta),
      reserve,
      saturating_deadline(start, std::max<int64_t>(0, soft_delta - reserve)),
  };
}

template <class T, class IterationFn, class MatrixFn>
T run_root_stages(T fallback, int first_depth, int last_depth, Clock &clock,
                  RequestDeadlines deadlines, IterationFn iteration,
                  MatrixFn matrix) {
  T staged = std::move(fallback);
  for (int depth = first_depth; depth <= last_depth; ++depth) {
    if (clock.now_ms() >= deadlines.search_stop)
      break;
    std::optional<T> completed = iteration(depth, deadlines.hard);
    if (!completed || clock.now_ms() >= deadlines.soft)
      break;
    staged = std::move(*completed);
  }
  // The search stop excludes the matrix reserve, so the fallback is always
  // entered even when no deeper search iteration completed.
  return matrix(std::move(staged), deadlines.hard);
}

} // namespace uttt
