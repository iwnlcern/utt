#pragma once

#include <cstdint>

#include "core/types.hpp"

namespace uttt {

struct RootContext {
  Seat seat;
  int64_t budget_x;
  int64_t budget_o;
};

enum class ChipResult : uint8_t { XWins, OWins, Draw };

constexpr ChipResult winner_on_chips(int64_t budget_x, int64_t budget_o) {
  return budget_x > budget_o ? ChipResult::XWins
       : budget_x < budget_o ? ChipResult::OWins
                             : ChipResult::Draw;
}

}  // namespace uttt
