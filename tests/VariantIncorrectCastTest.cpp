#include <assert.h>
#include <meiose/variant.hpp>

using namespace meiose;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  std::array<variant, 17> vars = {{
    nullptr,
    true,
    false,
    13.37f,
    90.01,
    static_cast<uint8_t>(1),
    static_cast<uint16_t>(2),
    static_cast<uint32_t>(3),
    static_cast<int8_t>(4),
    static_cast<int16_t>(5),
    static_cast<int32_t>(6),
    static_cast<int64_t>(7),
    variant::binary { 1, 2, 3 },
    variant::extension { 8, { 1, 2, 3 } },
    variant::string { "test" },
    variant::array { 1, 2, 3 },
    variant::map { {1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'} }
  }};

  for (size_t i = 0; i < vars.size(); ++i) {
    assert(vars[i].operator bool() == (i == 1));
  }

  const variant::binary defbin;
  for (size_t i = 0; i < vars.size(); ++i) {
    assert(i == 12 || vars[i].operator const binary&() == defbin);
  }

  const variant::extension defext;
  for (size_t i = 0; i < vars.size(); ++i) {
    assert(i == 13 || vars[i].operator const extension&() == defext);
  }

  for (size_t i = 0; i < vars.size(); ++i) {
    assert((i >= 3 && i <= 11) || vars[i].operator double() == double());
    assert((i >= 3 && i <= 11) || vars[i].operator int64_t() == int64_t());
  }

  const variant::string defstr;
  for (size_t i = 0; i < vars.size(); ++i) {
    assert(i == 14 || vars[i].operator const string&() == defstr);
  }

  const variant::array defarr;
  for (size_t i = 0; i < vars.size(); ++i) {
    assert(i == 15 || vars[i].operator const array&() == defarr);
  }

  const variant::map defmap;
  for (size_t i = 0; i < vars.size(); ++i) {
    assert(i == 16|| vars[i].operator const map&() == defmap);
  }

  return 0;
}
