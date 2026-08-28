#pragma once

#include <array>

namespace uttt {

// Hand-initialized c4 weights. X advantages lower the chip fraction X needs;
// O advantages raise it. Indices 11/12 are completable macro-line potential;
// 13/14 aggregate forced-board legal-edge destination richness. The fitter
// writes this same ordered feature ABI.
inline constexpr std::array<double, 15> kEvalWeights = {
    0.0, 0.0, -0.65, 0.65, 0.0, -0.055, 0.055, -0.020,
    0.020, -0.035, 0.035, -0.22, 0.22, -0.025, 0.025};

} // namespace uttt
