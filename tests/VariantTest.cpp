#include <assert.h>
#include <meiose/debug.hpp>
#include <meiose/variant.hpp>

#include <iostream>

#include "TestUtils.hpp"

using namespace meiose;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  std::cout << "sizeof(variant) = " << sizeof(variant) << '\n';

  std::mt19937 r;
  std::random_device rdev;
  std::mt19937::result_type seed = rdev();
  std::cout << "seed=" << seed << '\n';
  r.seed(seed);
  for (int i = 0; i < 8; ++i) {
    std::cout << debug::dump(randomVariant(r())) << '\n';
  }

  return 0;
}
