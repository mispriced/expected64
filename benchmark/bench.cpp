#include <algorithm>  // for std::shuffle
#include <optional>
#include <random>  // for std::mt19937 and std::random_device

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <expected64/expected64.hpp>

enum class error_code
{
  no_error = 0,
  error
};

std::vector<int> gen_shuffled_numbers()
{
  std::vector<int> numbers;
  for (int i = -32; i <= 32; ++i) {
    numbers.push_back(i);
  }

  std::random_device rd;
  std::mt19937       g(rd());
  std::shuffle(numbers.begin(), numbers.end(), g);

  return numbers;
}

expected64<int64_t, error_code> factorial_expected64(int n)
{
  if (n < 0) {
    return expected64<int64_t, error_code>(error_code::error);
  }
  int64_t result = 1;
  for (int i = 1; i <= n; ++i) {
    result *= i;
  }
  return expected64<int64_t, error_code>(result);
}

std::optional<int64_t> factorial_optional(int n)
{
  if (n < 0) {
    return {};
  }
  int64_t result = 1;
  for (int i = 1; i <= n; ++i) {
    result *= i;
  }
  return result;
}

int64_t factorial_raw(int n)
{
  if (n < 0) {
    return -1;
  }
  int64_t result = 1;
  for (int i = 1; i <= n; ++i) {
    result *= i;
  }
  return result;
}

TEST_CASE("factorial")
{
  std::vector<int> numbers = gen_shuffled_numbers();

  BENCHMARK("Factorial with expected64")
  {
    int64_t score = 0;
    for (int num : numbers) {
      auto result = factorial_expected64(num);
      score += result.has_error() ? 0 : result.get_value();
    }
    return score;
  };

  BENCHMARK("Factorial with std::optional")
  {
    int64_t score = 0;
    for (int num : numbers) {
      auto result = factorial_optional(num);
      score += result.has_value() ? result.value() : 0;
    }
    return score;
  };

  BENCHMARK("Factorial with Raw Type")
  {
    int64_t score = 0;
    for (int num : numbers) {
      auto result = factorial_raw(num);
      score += (result > 0) ? result : 0;
    }
    return score;
  };
}

auto cube_expected64(double value)
{
  if (value < 0) {
    return expected64<double, error_code>(error_code::error);
  }
  return expected64<double, error_code>(value * value * value);
}

std::optional<double> cube_optional(double value)
{
  if (value < 0) {
    return {};
  }
  return {value * value * value};
}

auto cube_raw(double value)
{
  if (value < 0) {
    return -1.0;
  }
  return value * value * value;
}

TEST_CASE("cube")
{
  std::vector<int> numbers = gen_shuffled_numbers();

  BENCHMARK("Cube with expected64")
  {
    int64_t score = 0;
    for (int num : numbers) {
      auto result = cube_expected64(num);
      score += result.has_error() ? 0 : result.get_value();
    }
    return score;
  };

  BENCHMARK("Cube with std::optional")
  {
    int64_t score = 0;
    for (int num : numbers) {
      auto result = cube_optional(num);
      score += result.has_value() ? result.value() : 0;
    }
    return score;
  };

  BENCHMARK("Cube with Raw Type")
  {
    int64_t score = 0;
    for (int num : numbers) {
      auto result = cube_raw(num);
      score += (result > 0) ? result : 0;
    }
    return score;
  };
}
