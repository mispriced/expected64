#include "expected64/expected64.hpp"

auto main() -> int
{
  auto const result = name();

  return result == "expected64" ? 0 : 1;
}
