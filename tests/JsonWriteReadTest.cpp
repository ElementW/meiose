#include <cassert>
#include <chrono>
#include <meiose/variant.hpp>
#include <meiose/json.hpp>

#include "TestUtils.hpp"

#include <meiose/debug.hpp>
#include <iostream>

#define TIMING

using namespace meiose;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  constexpr int runs = 4096;

  std::mt19937 r;
  std::random_device rdev;
  std::mt19937::result_type seed = rdev();
  std::cout << "seed=" << seed << '\n';
  r.seed(seed);
#ifdef TIMING
  auto start = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> read, write;
#endif
  for (int i = 0; i < runs; ++i) {
    using T = meiose::variant::Type;
    variant v = randomVariant(r(), true, { T::Nil, T::Bool, T::Int, T::Str, T::Array, T::Map });
#ifdef TIMING
    auto writestart = std::chrono::high_resolution_clock::now();
#endif
    std::string vstr = json::write(v);
#ifdef TIMING
    auto readstart = std::chrono::high_resolution_clock::now();
#endif
    variant nv = json::read(vstr);
#ifdef TIMING
    auto rwend = std::chrono::high_resolution_clock::now();
    write += readstart - writestart;
    read += rwend - readstart;
#endif
    if (nv != v) {
      std::cout <<
        debug::dump(v) <<
        "\n============================\n" <<
        json::write(v) <<
        "\n============================\n" <<
        debug::dump(nv) << std::endl;
      json::read(vstr);
      assert(false);
    }
  }
#ifdef TIMING
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end-start;
  std::cout <<
    "Total   " << diff.count() << " s\n" <<
    "Gen/cmp " << (diff-read-write).count() << " s\n" <<
    "R       " << read.count() << " s\n" <<
    "Per R   " << std::chrono::duration<double, std::milli>(read).count() / runs << " ms\n" <<
    "W       " << write.count() << " s\n" <<
    "Per W   " << std::chrono::duration<double, std::milli>(write).count() / runs << " ms\n";
#endif

  return 0;
}
