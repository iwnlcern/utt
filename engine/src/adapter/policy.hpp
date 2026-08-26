#pragma once

#include "adapter/wire.hpp"
#include "core/clock.hpp"

#include <cassert>

namespace uttt {

struct Policy {
  virtual wire::TurnReply choose(const wire::TurnRequest&, Clock&) = 0;
  virtual ~Policy() = default;
};

struct PlaceholderPolicy final : Policy {
  wire::TurnReply choose(const wire::TurnRequest& request, Clock& clock) override {
    assert(!request.legal.empty());
    (void)clock.now_ms();
    return {request.request_id,
            0,
            request.legal.front(),
            {"estimate", std::nullopt, std::nullopt, 0, true}};
  }
};

}  // namespace uttt
