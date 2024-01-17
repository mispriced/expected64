#include <atomic>
#include <fstream>

#include <nanobench.h>

#include "common.hpp"

int main()
{
  int y = 0;

  auto positive_five = 5;
  auto negative_five = -5;

  auto bench = [&](auto func, const char* description, int num)
  {
    std::ofstream out {std::string(description) + ".json"};
    ankerl::nanobench::Bench()
        .run(description,
             [&]
             {
               auto result = func(num);
               ankerl::nanobench::doNotOptimizeAway(result);
             })
        .render(ankerl::nanobench::templates::pyperf(), out);
  };

  bench(factorial_raw<double>, "factorial(5)-raw-double", positive_five);
  bench(factorial_raw<double>, "factorial(-5)-raw-double", negative_five);
  bench(factorial_optional<double>, "factorial(5)-optional-double", positive_five);
  bench(factorial_optional<double>, "factorial(-5)-optional-double", negative_five);
  bench(factorial_expected64<double>, "factorial(5)-expected64-double", positive_five);
  bench(factorial_expected64<double>, "factorial(-5)-expected64-double", negative_five);
}