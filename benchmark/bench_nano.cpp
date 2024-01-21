#include <atomic>
#include <fstream>

#include <nanobench.h>

#include "common.hpp"

int main()
{
  int y = 0;

  int64_t test_value = 5;

  auto bench = [&](auto func, const char* description, int num)
  {
    std::ofstream out {std::string(description) + ".json"};
    ankerl::nanobench::Bench()
        .minEpochIterations(1000000)
        .run(description,
             [&]
             {
               auto result = func(num);
               ankerl::nanobench::doNotOptimizeAway(result);
             })
        .render(ankerl::nanobench::templates::pyperf(), out);
  };

  bench(factorial_raw<int64_t>, "factorial-raw-int", test_value);
  bench(factorial_optional<int64_t>, "factorial-optional-int", test_value);
  bench(factorial_expected<int64_t>, "factorial-tlexpected-int", test_value);
  bench(factorial_expected64<int64_t>, "factorial-expected64-int", test_value);
}