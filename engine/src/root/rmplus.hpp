#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace uttt {

struct RMPlusResult {
  std::vector<double> row_strategy;
  std::vector<double> column_strategy;
  double value = 0.0;
  double average_regret = 0.0;
  double exploitability = 0.0;
  int iterations = 0;
};

namespace rmplus_detail {

inline std::vector<double> strategy(const std::vector<double> &regret) {
  const double total = std::accumulate(regret.begin(), regret.end(), 0.0);
  if (total > 0.0) {
    std::vector<double> result = regret;
    for (double &value : result)
      value /= total;
    return result;
  }
  return std::vector<double>(regret.size(), 1.0 / regret.size());
}

inline void normalize(std::vector<double> &values) {
  const double total = std::accumulate(values.begin(), values.end(), 0.0);
  if (total == 0.0) {
    std::fill(values.begin(), values.end(), 1.0 / values.size());
    return;
  }
  for (double &value : values)
    value /= total;
}

} // namespace rmplus_detail

inline RMPlusResult solve_rmplus(const std::vector<std::vector<double>> &matrix,
                                 int max_iterations = 10'000,
                                 std::function<bool()> stop = {}) {
  if (matrix.empty() || matrix.front().empty() || max_iterations < 0 ||
      std::any_of(
          matrix.begin(), matrix.end(),
          [&](const auto &row) {
            return row.size() != matrix.front().size() ||
                   std::any_of(row.begin(), row.end(), [](double value) {
                     return !std::isfinite(value) || value < -1.0 ||
                            value > 1.0;
                   });
          })) {
    throw std::invalid_argument("RM+ requires a nonempty rectangular matrix");
  }
  const std::size_t rows = matrix.size();
  const std::size_t columns = matrix.front().size();
  std::vector<double> row_regret(rows, 0.0);
  std::vector<double> column_regret(columns, 0.0);
  std::vector<double> row_average(rows, 0.0);
  std::vector<double> column_average(columns, 0.0);
  int completed = 0;
  for (; completed < max_iterations; ++completed) {
    if (stop && stop())
      break;
    const auto x = rmplus_detail::strategy(row_regret);
    const auto y = rmplus_detail::strategy(column_regret);
    for (std::size_t i = 0; i < rows; ++i)
      row_average[i] += x[i];
    for (std::size_t j = 0; j < columns; ++j)
      column_average[j] += y[j];

    std::vector<double> row_values(rows, 0.0);
    std::vector<double> column_values(columns, 0.0);
    for (std::size_t i = 0; i < rows; ++i) {
      for (std::size_t j = 0; j < columns; ++j) {
        row_values[i] += matrix[i][j] * y[j];
        column_values[j] += x[i] * matrix[i][j];
      }
    }
    const double expected =
        std::inner_product(x.begin(), x.end(), row_values.begin(), 0.0);
    for (std::size_t i = 0; i < rows; ++i)
      row_regret[i] = std::max(0.0, row_regret[i] + row_values[i] - expected);
    for (std::size_t j = 0; j < columns; ++j)
      column_regret[j] =
          std::max(0.0, column_regret[j] + expected - column_values[j]);
  }
  rmplus_detail::normalize(row_average);
  rmplus_detail::normalize(column_average);

  std::vector<double> row_values(rows, 0.0);
  std::vector<double> column_values(columns, 0.0);
  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < columns; ++j) {
      row_values[i] += matrix[i][j] * column_average[j];
      column_values[j] += row_average[i] * matrix[i][j];
    }
  }
  RMPlusResult result;
  result.row_strategy = std::move(row_average);
  result.column_strategy = std::move(column_average);
  result.value =
      std::inner_product(result.row_strategy.begin(), result.row_strategy.end(),
                         row_values.begin(), 0.0);
  const double best_row =
      *std::max_element(row_values.begin(), row_values.end());
  const double best_column =
      *std::min_element(column_values.begin(), column_values.end());
  result.exploitability = best_row - best_column;
  result.average_regret =
      completed == 0
          ? result.exploitability
          : std::max(
                *std::max_element(row_regret.begin(), row_regret.end()),
                *std::max_element(column_regret.begin(), column_regret.end())) /
                completed;
  result.iterations = completed;
  return result;
}

} // namespace uttt
