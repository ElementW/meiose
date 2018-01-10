#include <cassert>
#include <chrono>
#include <meiose/variant.hpp>
#include <meiose/msgpack.hpp>

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
    variant v = randomVariant(r());
#ifdef TIMING
    auto writestart = std::chrono::high_resolution_clock::now();
#endif
    std::string vstr = msgpack::write(v);
#ifdef TIMING
    auto readstart = std::chrono::high_resolution_clock::now();
#endif
    variant nv = msgpack::read(vstr);
#ifdef TIMING
    auto rwend = std::chrono::high_resolution_clock::now();
    write += readstart - writestart;
    read += rwend - readstart;
#endif
    if (nv != v) {
      std::cout <<
        debug::dump(v) <<
        "\n============================\n" <<
        debug::dump(nv) << std::endl;
      vstr = msgpack::write(v);
      msgpack::read(vstr);
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
