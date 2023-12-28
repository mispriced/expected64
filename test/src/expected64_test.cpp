#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "expected64/expected64.hpp"

#include <catch2/catch_all.hpp>

using namespace Catch;

// Define an enum for error codes
enum class error_code
{
  no_error = 0,
  calculation_error,
  misc_error
};

// Example function that uses expected64 with enum for error
expected64<int64_t, error_code> perform_calculation(bool succeed)
{
  if (succeed) {
    return expected64<int64_t, error_code>(12345);  // Operation succeeded
  } else {
    return expected64<int64_t, error_code>(error_code::calculation_error);  // Operation failed
  }
}

// Define a function that performs a calculation and returns an expected64 result
expected64<int64_t, error_code> perform_overflow_prone_calculation(uint64_t value)
{
  if (value > std::numeric_limits<int64_t>::max()) {
    return expected64<int64_t, error_code>(error_code::calculation_error);
  }

  int64_t result = static_cast<int64_t>(value);  // Cast could overflow on large uint64_t values
  return expected64<int64_t, error_code>(result);
}

void someFunc() {}  // used in function pointer-test

// Vanilla tests
TEST_CASE("perform_calculation Tests")
{
  SECTION("Calculation Succeeds")
  {
    auto result = perform_calculation(true);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == 12345);
  }

  SECTION("Calculation Fails")
  {
    auto result = perform_calculation(false);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::calculation_error);
  }
}

// Type by type tests
TEST_CASE("uint_64t tests")
{
  constexpr uint64_t max_uint64 = std::numeric_limits<uint64_t>::max();
  constexpr uint64_t largest_valid = max_uint64 >> 1;

  //  max_uint64 = 2^64 - 1
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 111111111111111111111111111111111111111111111111111111111111111 |
  //  |---------------------------------------------------------------------|

  SECTION("Maximum uint64_t is an error")
  {
    auto result = expected64<uint64_t, error_code>(max_uint64);
    REQUIRE(result.has_error());
    // The error code is however meaningless
  }

  //  largest_valid = 2^63 - 1
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 0 | 111111111111111111111111111111111111111111111111111111111111111 |
  //  |---------------------------------------------------------------------|

  SECTION("Largest valid uint64_t is not an error")
  {
    auto result = expected64<uint64_t, error_code>(largest_valid);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == largest_valid);
  }

  //  calculation error
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 000000000000000000000000000000000000000000000000000000000000001 |
  //  |---------------------------------------------------------------------|

  SECTION("Setting an error")
  {
    auto result = expected64<uint64_t, error_code>(largest_valid);
    result.set_error(error_code::calculation_error);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::calculation_error);
  }

  //  misc_error error
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 000000000000000000000000000000000000000000000000000000000000010 |
  //  |---------------------------------------------------------------------|

  SECTION("Constructing an error")
  {
    auto result = expected64<uint64_t, error_code>(error_code::misc_error);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::misc_error);
  }
}

TEST_CASE("int64_t tests")
{
  constexpr int64_t max_int64 = std::numeric_limits<int64_t>::max();
  constexpr int64_t min_int64 = std::numeric_limits<int64_t>::min();
  constexpr int64_t largest_valid = max_int64 >> 2;
  constexpr int64_t smallest_valid = min_int64 / 4;  // -2^61

  //  max_int64 = 2^63 - 1
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 0 | 1 | 11111111111111111111111111111111111111111111111111111111111 |
  //  |---------------------------------------------------------------------|

  SECTION("Maximum int64_t sets error b it")
  {
    auto result = expected64<int64_t, error_code>(max_int64);
    REQUIRE(result.has_error());
    // The error code is however meaningless
  }

  //  min_int64 = -(2^63)
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 0 | 00000000000000000000000000000000000000000000000000000000000 |
  //  |---------------------------------------------------------------------|

  SECTION("Minimum int64_t overflows the error flag (0 for negatives)")
  {
    auto result = expected64<int64_t, error_code>(min_int64);
    REQUIRE(result.has_error());
  }

  //  largest_valid = 2^62 -1
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 0 | 0 | 11111111111111111111111111111111111111111111111111111111111 |
  //  |---------------------------------------------------------------------|

  SECTION("Largest valid int64_t is not an error")
  {
    auto result = expected64<int64_t, error_code>(largest_valid);
    REQUIRE(!result.has_error());  // No error should be flagged
    REQUIRE(result.get_value() == largest_valid);
  }

  //  smallest_valid = -(2^61)
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 0 | 11111111111111111111111111111111111111111111111111111111111 |
  //  |---------------------------------------------------------------------|
  SECTION("Smallest valid int64_t is not an error")
  {
    auto result = expected64<int64_t, error_code>(smallest_valid);
    REQUIRE(!result.has_error());  // No error should be flagged
    REQUIRE(result.get_value() == smallest_valid);
  }

  //  misc_error error
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 000000000000000000000000000000000000000000000000000000000000010 |
  //  |---------------------------------------------------------------------|

  SECTION("Setting an error")
  {
    auto result = expected64<int64_t, error_code>(largest_valid);
    result.set_error(error_code::misc_error);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::misc_error);
  }

  //  calculation error
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 000000000000000000000000000000000000000000000000000000000000001 |
  //  |---------------------------------------------------------------------|

  SECTION("Constructing an error")
  {
    auto result = expected64<int64_t, error_code>(error_code::calculation_error);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::calculation_error);
  }

  //  -1234567
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 |  00101101110010010110110111111111111111111111111111111111111111 |
  //  |---------------------------------------------------------------------|

  SECTION("Constructing a valid odd negative number")
  {
    auto result = expected64<int64_t, error_code>(-1234567);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == -1234567);
  }

  // Error set in a negative number
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 1 | 00101101110010010110110111111111111111111111111111111111111 |
  //  |---------------------------------------------------------------------|

  SECTION("Setting an error in a negative number")
  {
    auto result = expected64<int64_t, error_code>(-1234567);
    result.set_error(error_code::misc_error);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::misc_error);
  }

  // Zero value
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 0 | 0 | 00000000000000000000000000000000000000000000000000000000000 |
  //  |---------------------------------------------------------------------|

  SECTION("Zero Integer Value")
  {
    auto result = expected64<int64_t, error_code>(0);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == 0);
  }
}

TEST_CASE("double tests")
{
  constexpr double nan_value = std::numeric_limits<double>::quiet_NaN();
  constexpr double inf_value = std::numeric_limits<double>::infinity();
  constexpr double ninf_value = -std::numeric_limits<double>::infinity();
  const double     max_double = std::numeric_limits<double>::max();
  const double     min_double = std::numeric_limits<double>::min();  // min positive value

  //  63                                                                  0
  // |---------------------------------------------------------------------|
  // | S | 11111111111 | Non-zero fraction (not all zeros)                 |
  // |---------------------------------------------------------------------|

  SECTION("NaN Value")
  {
    auto result = expected64<double, error_code>(nan_value);
    REQUIRE(result.has_error());
    REQUIRE(std::isnan(result.get_value()));
  }

  //   63                                                                  0
  // |---------------------------------------------------------------------|
  // | 0 | 11111111111 | 0000000000000000000000000000000000000000000000000 |
  // |---------------------------------------------------------------------|

  SECTION("Positive Infinity")
  {
    auto result = expected64<double, error_code>(inf_value);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(inf_value));
  }

  //   63                                                                  0
  // |---------------------------------------------------------------------|
  // | 1 | 11111111111 | 0000000000000000000000000000000000000000000000000 |
  // |---------------------------------------------------------------------|

  SECTION("Negative Infinity")
  {
    auto result = expected64<double, error_code>(ninf_value);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(ninf_value));
  }

  // Zero Double Value
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 0 | 00000000000 | 0000000000000000000000000000000000000000000000000 |
  //  |---------------------------------------------------------------------|

  SECTION("Zero Double Value")
  {
    auto result = expected64<double, error_code>(0.0);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(0.0));
  }

  // Maximum Double Value
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 0 | 11111111110 | 1111111111111111111111111111111111111111111111111 |
  //  |---------------------------------------------------------------------|

  SECTION("Maximum Double Value")
  {
    auto result = expected64<double, error_code>(max_double);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(max_double));
  }

  // Minimum Positive Double Value
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 0 | 00000000000 | 0000000000000000000000000000000000000000000000001 |
  //  |---------------------------------------------------------------------|

  SECTION("Minimum Positive Double Value")
  {
    auto result = expected64<double, error_code>(min_double);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(min_double));
  }

  // Error Handling for Double (Example: calculation_error encoded in NaN bits)
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | S | 11111111111 | encoded error code bits                           |
  //  |---------------------------------------------------------------------|

  SECTION("Error Handling for Double")
  {
    auto result = expected64<double, error_code>(error_code::calculation_error);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::calculation_error);

    result = expected64<double, error_code>(12345);
    result.set_error(error_code::misc_error);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::misc_error);
  }

  // Subnormal Double Value
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 0 | 00000000000 | very small non-zero fraction                      |
  //  |---------------------------------------------------------------------|

  SECTION("Subnormal Double Value")
  {
    double subnormal_value = std::numeric_limits<double>::denorm_min();
    auto   result = expected64<double, error_code>(subnormal_value);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(subnormal_value));
  }

  // Precision of Double Values
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | Approximate binary representation of π                              |
  //  |---------------------------------------------------------------------|

  SECTION("Precision of Double Values")
  {
    double precise_value = 3.14159265358979323846;  // π with maximum double precision
    auto   result = expected64<double, error_code>(precise_value);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(precise_value));
  }

  // Negative Double Value
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | exponent bits | fraction bits representing -2.7182818284        |
  //  |---------------------------------------------------------------------|

  SECTION("Negative double value")
  {
    double some_negative = -2.7182818284;
    auto   result = expected64<double, error_code>(some_negative);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(some_negative));
  }

  // Handling Denormalized Doubles
  SECTION("Denormalized Negative Double Value")
  {
    double denorm_neg_double = -std::numeric_limits<double>::denorm_min();
    auto   result = expected64<double, error_code>(denorm_neg_double);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(denorm_neg_double));
  }

  // Comparing Different NaN Representations
  SECTION("Different NaN Representations")
  {
    double signaling_nan = std::numeric_limits<double>::signaling_NaN();
    auto   result = expected64<double, error_code>(signaling_nan);
    REQUIRE(result.has_error());
  }

  // Rounding Error Test
  SECTION("Rounding Error for Binary-Problematic Values")
  {
    double problematic_value = 0.1;
    auto   result = expected64<double, error_code>(problematic_value);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(problematic_value));
  }

  // Extreme Exponent Values
  SECTION("Double with Extreme Exponent")
  {
    double extreme_exponent_val = std::ldexp(1.0, 1023);  // Largest exponent for double
    auto   result = expected64<double, error_code>(extreme_exponent_val);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == Approx(extreme_exponent_val));
  }
}

TEST_CASE("Pointer Tests")
{
  int  value;
  int* aligned_ptr = &value;
  int* unaligned_ptr = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(aligned_ptr) + 1);

  // Aligned Pointer (assuming it's aligned such that LSB is 0)
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  |   X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | 0 |
  //  |---------------------------------------------------------------------|

  SECTION("Aligned Pointer")
  {
    auto result = expected64<int*, error_code>(aligned_ptr);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == aligned_ptr);
  }

  // Unaligned Pointer (assuming LSB is set due to unalignment)
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  |   X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | 1 |
  //  |---------------------------------------------------------------------|

  SECTION("Unaligned Pointer")
  {
    auto result = expected64<int*, error_code>(unaligned_ptr);
    REQUIRE(result.has_error());
    // We set the LSB by accident!
  }

  // Null Pointer
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  |   0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
  //  |---------------------------------------------------------------------|

  SECTION("Null Pointer")
  {
    auto result = expected64<int*, error_code>(nullptr);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == nullptr);
  }

  // Error set in an aligned pointer (assuming LSB is not set)
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  |   X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | 1 |
  //  |---------------------------------------------------------------------|

  SECTION("Error in Aligned Pointer")
  {
    auto result = expected64<int*, error_code>(aligned_ptr);
    result.set_error(error_code::misc_error);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::misc_error);
  }

  SECTION("Different Alignment Pointer")
  {
    alignas(16) int aligned_value;
    int*            aligned_ptr = &aligned_value;
    auto            result = expected64<int*, error_code>(aligned_ptr);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == aligned_ptr);
  }

  SECTION("Function Pointer Test")
  {
    auto func_ptr = &someFunc;  // Assuming someFunc is defined
    auto result = expected64<decltype(func_ptr), error_code>(func_ptr);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == func_ptr);
  }

  // Array Pointers
  SECTION("Array Pointer Arithmetic")
  {
    int  array[10];
    int* array_ptr = array;
    array_ptr++;  // Increment pointer
    auto result = expected64<int*, error_code>(array_ptr);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == array_ptr);
  }

  // Null Pointer with Different Representations
  SECTION("Different Null Pointer Representations")
  {
    int* null_ptr = nullptr;
    auto result = expected64<int*, error_code>(null_ptr);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == nullptr);
  }

  // Pointers to Volatile and Const
  SECTION("Pointers to Volatile and Const")
  {
    const volatile int  cv_int = 10;
    const volatile int* cv_ptr = &cv_int;
    auto                result = expected64<const volatile int*, error_code>(cv_ptr);
    REQUIRE(!result.has_error());
    REQUIRE(result.get_value() == cv_ptr);
  }
}

// Overflow testing
TEST_CASE("Error Handling and Overflows")
{
  constexpr uint64_t large_value = static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1;

  // Overflow attempt
  //   63                                                                  0
  //  |---------------------------------------------------------------------|
  //  | 1 | 1 | 00000000000000000000000000000000000000000000000000000000000 |
  //  |---------------------------------------------------------------------|

  SECTION("Error on Overflow Attempt")
  {
    auto result = perform_overflow_prone_calculation(large_value);
    REQUIRE(result.has_error());
    REQUIRE(result.get_error() == error_code::calculation_error);
  }
}
