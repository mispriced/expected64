#include <algorithm>  // for std::shuffle
#include <optional>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include "common.hpp"

template<typename T>
void run_factorial_benchmarks()
{
  std::vector<int> numbers = gen_shuffled_numbers();

  BENCHMARK("Factorial with expected64")
  {
    T score = 0;
    for (int num : numbers) {
      auto result = factorial_expected64<T>(num);
      score += result.has_error() ? 0 : result.get_value();
    }
    return score;
  };

  BENCHMARK("Factorial with std::optional")
  {
    T score = 0;
    for (int num : numbers) {
      auto result = factorial_optional<T>(num);
      score += result.has_value() ? result.value() : 0;
    }
    return score;
  };

  BENCHMARK("Factorial with Raw Type")
  {
    T score = 0;
    for (int num : numbers) {
      auto result = factorial<T>(num);
      score += (result > 0) ? result : 0;
    }
    return score;
  };
}

TEST_CASE("factorial - int64_t")
{
  run_factorial_benchmarks<int64_t>();
}

TEST_CASE("factorial - uint64_t")
{
  run_factorial_benchmarks<uint64_t>();
}

TEST_CASE("factorial - double")
{
  run_factorial_benchmarks<double>();
}

template<typename T>
void run_cube_benchmarks()
{
  std::vector<int> numbers = gen_shuffled_numbers();

  BENCHMARK("Cube with expected64")
  {
    T score = 0;
    for (int num : numbers) {
      auto result = cube_expected64<T>(static_cast<T>(num));
      score += result.has_error() ? 0 : result.get_value();
    }
    return score;
  };

  BENCHMARK("Cube with std::optional")
  {
    T score = 0;
    for (int num : numbers) {
      auto result = cube_optional<T>(static_cast<T>(num));
      score += result.has_value() ? result.value() : 0;
    }
    return score;
  };

  BENCHMARK("Cube with Raw Type")
  {
    T score = 0;
    for (int num : numbers) {
      auto result = cube<T>(static_cast<T>(num));
      score += result;
    }
    return score;
  };
}

TEST_CASE("cube - int64_t")
{
  run_cube_benchmarks<int64_t>();
}

TEST_CASE("cube - uint64_t")
{
  run_cube_benchmarks<uint64_t>();
}

TEST_CASE("cube - double")
{
  run_cube_benchmarks<double>();
}