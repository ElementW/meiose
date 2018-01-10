#include <assert.h>
#include <meiose/variant.hpp>

#include "TestUtils.hpp"

using namespace meiose;

void testClear() {
  variant
    vbool(true),
    vfloat(9999.),
    vint(9999),
    vbin(variant::binary { 1, 2, 3 }),
    vext(variant::extension { 8, { 1, 2, 3 } }),
    vstr(variant::string { "test" }),
    varr(variant::array { 1, 2, 3 }),
    vmap(variant::map { {1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'} });

  assert(static_cast<bool>(vbool) == true);
  vbool.clear();
  assert(static_cast<bool>(vbool) == false);

  assert(nearlyEqual(static_cast<double>(vfloat), 9999.));
  vfloat.clear();
  assert(static_cast<double>(vfloat) == 0.);

  assert(static_cast<int>(vint) == 9999);
  vint.clear();
  assert(static_cast<int>(vint) == 0);

  assert(static_cast<const binary&>(vbin).size() == 3);
  vbin.clear();
  assert(static_cast<const binary&>(vbin).empty());

  assert(static_cast<const extension&>(vext).first == 8);
  assert(static_cast<const extension&>(vext).second.size() == 3);
  vext.clear();
  assert(static_cast<const extension&>(vext).first == 0);
  assert(static_cast<const extension&>(vext).second.empty());

  assert(static_cast<const string&>(vstr) == "test");
  vstr.clear();
  assert(static_cast<const string&>(vstr).empty());

  assert(static_cast<const array&>(varr).size() == 3);
  varr.clear();
  assert(static_cast<const array&>(varr).empty());

  assert(static_cast<const map&>(vmap).size() == 4);
  vmap.clear();
  assert(static_cast<const map&>(vmap).empty());
}

void testPut() {
  variant v(variant::Type::Map);
  v.put(1, 2);
  v.put(nullptr, "str");
  assert(v.getMap().at(1) == 2);
  assert(v.getMap().at(nullptr) == "str");

  variant bad;
  try {
    bad.put(0, 0);
    assert(false);
  } catch(const std::runtime_error&) {
  }
}

void testMapIndex() {
  variant v(variant::Type::Map);
  v.put(1, 2);
  v.put(nullptr, "str");
  v.put("strkey", 0xDEADBEEF);
  // non-const
  assert(v[1] == 2);
  assert(v[static_cast<uint8_t>(1)] == 2);
  assert(v[static_cast<uint16_t>(1)] == 2);
  assert(v[static_cast<uint32_t>(1)] == 2);
  assert(v[static_cast<int8_t>(1)] == 2);
  assert(v[static_cast<int16_t>(1)] == 2);
  assert(v[static_cast<int32_t>(1)] == 2);
  assert(v[static_cast<int64_t>(1)] == 2);
  assert(v[nullptr] == "str");
  assert(v["strkey"] == 0xDEADBEEF);
  assert(v[std::string("strkey")] == 0xDEADBEEF);
  // const
  const variant &m = v;
  assert(m[1] == 2);
  assert(m[static_cast<uint8_t>(1)] == 2);
  assert(m[static_cast<uint16_t>(1)] == 2);
  assert(m[static_cast<uint32_t>(1)] == 2);
  assert(m[static_cast<int8_t>(1)] == 2);
  assert(m[static_cast<int16_t>(1)] == 2);
  assert(m[static_cast<int32_t>(1)] == 2);
  assert(m[static_cast<int64_t>(1)] == 2);
  assert(m[nullptr] == "str");
  assert(m["strkey"] == 0xDEADBEEF);
  assert(m[std::string("strkey")] == 0xDEADBEEF);
}

void testArrayIndex() {
  variant v(variant::array { 8, 2, 9, 4 });
  // non-const
  assert(v[0] == 8);
  assert(v[1] == 2);
  assert(v[variant(2)] == 9);
  assert(v[static_cast<uint8_t>(1)] == 2);
  assert(v[static_cast<uint16_t>(1)] == 2);
  assert(v[static_cast<uint32_t>(1)] == 2);
  assert(v[static_cast<int8_t>(1)] == 2);
  assert(v[static_cast<int16_t>(1)] == 2);
  assert(v[static_cast<int32_t>(1)] == 2);
  assert(v[static_cast<int64_t>(1)] == 2);
  assert_val_exception(v[nullptr] != "str", const std::runtime_error&);
  assert_val_exception(v["badkey"] != "str", const std::runtime_error&);
  assert_val_exception(v[-1] != "str", const std::runtime_error&);
  assert_val_exception(v[variant(-1)] != "str", const std::runtime_error&);
  // const
  const variant &m = v;
  assert(m[0] == 8);
  assert(m[1] == 2);
  assert(m[variant(2)] == 9);
  assert(m[static_cast<uint8_t>(1)] == 2);
  assert(m[static_cast<uint16_t>(1)] == 2);
  assert(m[static_cast<uint32_t>(1)] == 2);
  assert(m[static_cast<int8_t>(1)] == 2);
  assert(m[static_cast<int16_t>(1)] == 2);
  assert(m[static_cast<int32_t>(1)] == 2);
  assert(m[static_cast<int64_t>(1)] == 2);
  assert_val_exception(m[nullptr] != "str", const std::runtime_error&);
  assert_val_exception(m["badkey"] != "str", const std::runtime_error&);
  assert_val_exception(m[-1] != "str", const std::runtime_error&);
  assert_val_exception(m[variant(-1)] != "str", const std::runtime_error&);
}

void testBadIndex() {
  variant v;
  // non-const
  assert_val_exception(v[0] != "str", const std::runtime_error&);
  assert_val_exception(v[0U] != "str", const std::runtime_error&);
  assert_val_exception(v[nullptr] != "str", const std::runtime_error&);
  assert_val_exception(v["badkey"] != "str", const std::runtime_error&);
  assert_val_exception(v[-1] != "str", const std::runtime_error&);
  assert_val_exception(v[variant(-1)] != "str", const std::runtime_error&);
  // const
  const variant &m = v;
  assert_val_exception(m[0] != "str", const std::runtime_error&);
  assert_val_exception(m[0U] != "str", const std::runtime_error&);
  assert_val_exception(m[nullptr] != "str", const std::runtime_error&);
  assert_val_exception(m["badkey"] != "str", const std::runtime_error&);
  assert_val_exception(m[-1] != "str", const std::runtime_error&);
  assert_val_exception(m[variant(-1)] != "str", const std::runtime_error&);
}

void testSize() {
  assert(variant().size() == 0);
  assert(variant(nullptr).size() == 0);
  assert(variant(1).size() == 0);
  assert(variant(1.).size() == 0);
  assert(variant(false).size() == 0);
  assert(variant(true).size() == 0);
  assert(variant(variant::binary { 1, 2, 3 }).size() == 3);
  assert(variant(variant::extension { 8, { 1, 2, 3, 4, 5 } }).size() == 5);
  assert(variant(variant::string { "test" }).size() == 4);
  assert(variant(variant::array { 1, 2, 3 }).size() == 3);
  assert(variant(variant::map { {1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'} }).size() == 4);
}

void testPush() {
  variant v(variant::array { 8, 2, 9, 4 });
  v.push(-8);
  assert(v[4] == -8);
  variant vbackup = v;
  v.push(v);
  assert(v[5] == vbackup);

  variant bad;
  try {
    bad.push("temp construct = move");
    assert(false);
  } catch(const std::runtime_error&) {
  }
  try {
    const variant itsme("mario");
    bad.push(itsme);
    assert(false);
  } catch(const std::runtime_error&) {
  }
}

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  testClear();
  testPut();
  testMapIndex();
  testArrayIndex();
  testBadIndex();
  testSize();
  testPush();

  return 0;
}
