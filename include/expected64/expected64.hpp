#pragma once
#include <cmath>  // std::isnan
#include <cstdint>
#include <limits>  // std::numeric_limits
#include <string>
/**
 * @brief Tagged union for 64-bit expected value
 */

template<typename T>
concept Expected64Type =
    std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t> || std::is_same_v<T, double> || std::is_pointer_v<T>;

template<Expected64Type T, typename E>
class expected64
{
  static_assert(sizeof(T) == 8, "T should be 64-bit");
  static_assert(sizeof(E) < sizeof(T), "E should be smaller than T");
  static_assert(std::is_trivially_destructible<T>::value, "T must be trivially destructible");
  static_assert(std::is_trivially_destructible<E>::value, "E must be trivially destructible");

  union
  {
    T value;
    E error;
  };

  static constexpr uint64_t int64_error_flag = static_cast<uint64_t>(1) << 62;  // Bit 62 as error flag for int64_t
  static constexpr uint64_t uint64_error_flag = static_cast<uint64_t>(1) << 63;  // MSB as error flag for uint64_t
  static constexpr uint64_t ptr_error_flag = 1;  // LSB as error flag for pointers
  static constexpr uint64_t nan_mask = 0xFFF8'0000'0000'0000;  // Create a quiet NaN and preserve space for error code
public:
  expected64(T val) noexcept
      : value(val)
  {
  }

  expected64(E error_value) noexcept { set_error(error_value); }

  void set_error(E error_value)
  {
    error = error_value;
    if constexpr (std::is_same_v<T, double>) {
      double   nan_val = std::numeric_limits<double>::quiet_NaN();
      uint64_t nan_bits = *reinterpret_cast<const uint64_t*>(&nan_val);
      nan_bits = (nan_bits & nan_mask) | static_cast<uint64_t>(error_value);
      value = *reinterpret_cast<double*>(&nan_bits);
    } else {
      reinterpret_cast<uint64_t&>(value) = static_cast<uint64_t>(error_value);
      if constexpr (std::is_same_v<T, int64_t>) {
        reinterpret_cast<uint64_t&>(value) |= int64_error_flag;
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        reinterpret_cast<uint64_t&>(value) |= uint64_error_flag;
      } else if constexpr (std::is_pointer_v<T>) {
        reinterpret_cast<uint64_t&>(value) |= ptr_error_flag;
      }
    }
  }

  [[nodiscard]] inline bool has_error() const noexcept
  {
    if constexpr (std::is_same_v<T, double>) {
      return std::isnan(value);
    } else if constexpr (std::is_same_v<T, int64_t>) {
      uint64_t raw_bits = reinterpret_cast<const uint64_t&>(value);
      bool     isNegative = (raw_bits & (static_cast<uint64_t>(1) << 63)) != 0;
      bool     isErrorBitSet = (raw_bits & int64_error_flag) != 0;

      // For negative values, error if MSB+1 is NOT set (0), else no error if set (1)
      return isNegative ? !isErrorBitSet : isErrorBitSet;
    } else if constexpr (std::is_same_v<T, uint64_t> || std::is_pointer_v<T>) {
      uint64_t error_flag = std::is_same_v<T, uint64_t> ? uint64_error_flag : ptr_error_flag;
      return (reinterpret_cast<const uint64_t&>(value) & error_flag) != 0;
    }
  }

  [[nodiscard]] inline T get_value() const noexcept { return value; }

  [[nodiscard]] inline E get_error() const noexcept
  {
    if constexpr (std::is_same_v<T, double>) {
      uint64_t error_code_bits = reinterpret_cast<const uint64_t&>(value) & ~nan_mask;
      return static_cast<E>(error_code_bits);
    } else if constexpr (std::is_same_v<T, int64_t>) {
      if (value >= 0) {  // Positive int64
        return static_cast<E>(reinterpret_cast<const uint64_t&>(value) & ~int64_error_flag);
      } else {  // Negative int64
        return static_cast<E>(reinterpret_cast<const uint64_t&>(value) & ~ptr_error_flag);
      }
    } else if constexpr (std::is_same_v<T, uint64_t>) {
      return static_cast<E>(reinterpret_cast<const uint64_t&>(value) & ~uint64_error_flag);
    } else if constexpr (std::is_pointer_v<T>) {
      return static_cast<E>(reinterpret_cast<const uintptr_t&>(value) & ~ptr_error_flag);
    }
  }
};
