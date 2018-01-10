#include <assert.h>
#include <meiose/variant.hpp>

#include "TestUtils.hpp"

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

  assert(vars[3].asInt() == 13);
  assert(vars[3].asNumber<int>() == 13);
  assert(vars[4].asInt() == 90);
  assert(vars[4].asNumber<int>() == 90);
  assert(nearlyEqual(vars[3].asFloat(), 13.37f));
  assert(nearlyEqual(vars[3].asNumber<float>(), 13.37f));
  assert(nearlyEqual(vars[3].asNumber<double>(), 13.37f));
  assert(nearlyEqual(vars[3].asNumber<long double>(), 13.37f));
  assert(nearlyEqual(vars[4].asFloat(), 90.01));
  assert(nearlyEqual(vars[4].asNumber<float>(), 90.01));
  assert(nearlyEqual(vars[4].asNumber<double>(), 90.01));
  assert(nearlyEqual(vars[4].asNumber<long double>(), 90.01));

  for (size_t n = 5; n <= 11; ++n) {
    int i = static_cast<int>(n);
    assert(vars[n].asInt() == (i - 4));
    assert(vars[n].asNumber<int>() == (i - 4));
    assert(static_cast<uint8_t> (vars[n]) == (i - 4));
    assert(static_cast<uint16_t>(vars[n]) == (i - 4));
    assert(static_cast<uint32_t>(vars[n]) == static_cast<uint32_t>(i - 4));
    assert(static_cast<int8_t>  (vars[n]) == (i - 4));
    assert(static_cast<int16_t> (vars[n]) == (i - 4));
    assert(static_cast<int32_t> (vars[n]) == (i - 4));
    assert(static_cast<int64_t> (vars[n]) == (i - 4));
    assert(vars[n] == static_cast<uint8_t> (i - 4));
    assert(vars[n] == static_cast<uint16_t>(i - 4));
    assert(vars[n] == static_cast<uint32_t>(i - 4));
    assert(vars[n] == static_cast<int8_t>  (i - 4));
    assert(vars[n] == static_cast<int16_t> (i - 4));
    assert(vars[n] == static_cast<int32_t> (i - 4));
    assert(vars[n] == static_cast<int64_t> (i - 4));
    assert(vars[n] == (i - 4));
    assert(nearlyEqual(vars[n].asFloat(), i - 4));
    assert(nearlyEqual(vars[n].asNumber<float>(), i - 4));
    assert(nearlyEqual(vars[n].asNumber<double>(), i - 4));
    assert(nearlyEqual(vars[n].asNumber<long double>(), i - 4));
  }

  return 0;
}
