#pragma once
#include <optional>
#include <random>  // for std::mt19937 and std::random_device
#include <vector>

#include <expected64/expected64.hpp>
#include <tl/expected.hpp>

std::vector<int> gen_shuffled_numbers()
{
  std::vector<int> numbers;
  for (int i = -10; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      numbers.push_back(i);
    }
  }

  std::random_device rd;
  std::mt19937       g(rd());
  std::shuffle(numbers.begin(), numbers.end(), g);

  return numbers;
}

enum class error_code
{
  no_error = 0,
  error
};

template<typename T>
T factorial(T n)
{
  if (n < 0 || n > 50)
    return static_cast<T>(-1);  // Error case for signed types
  T result = 1;
  for (T i = 1; i <= n; ++i) {
    result *= i;
  }
  return result;
}

template<typename T>
T factorial_raw(T n)
{
  if (n < 0)
    return static_cast<T>(-1);  // Error case for signed types
  return factorial(n);
}

template<typename T>
std::optional<T> factorial_optional(T n)
{
  if (n < 0)
    return std::nullopt;
  return factorial(n);
}

template<typename T>
tl::expected<T, error_code> factorial_expected(T n)
{
  if (n < 0)
    return tl::unexpected {error_code::error};
  return factorial(n);
}

template<typename T>
expected64<T, error_code> factorial_expected64(T n)
{
  if (n < 0)
    return expected64<T, error_code>(error_code::error);
  return expected64<T, error_code>(factorial(n));
}

template<typename T>
T cube(const T& value)
{
  if (value < 0)
    return static_cast<T>(0);  // Error case for signed types
  return value * value * value;
}

template<typename T>
std::optional<T> cube_optional(const T& value)
{
  if (value < 0)
    return std::nullopt;
  return cube(value);
}

template<typename T>
expected64<T, error_code> cube_expected64(const T& value)
{
  if (value < 0)
    return expected64<T, error_code>(error_code::error);
  return expected64<T, error_code>(cube(value));
}