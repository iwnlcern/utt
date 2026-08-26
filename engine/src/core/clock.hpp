#pragma once

#include <chrono>
#include <cstdint>

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

}  // namespace uttt
