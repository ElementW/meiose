#include <assert.h>
#include <meiose/variant.hpp>

using namespace meiose;

void testCopy() {
  const variant::binary bin { 1, 2, 3 };
  const variant::extension ext { 8, { 1, 2, 3 } };
  const variant::string str { "test" };
  const char *cstr = "test";
  const variant::array arr { 1, 2, 3 };
  const variant::map map { {1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'} };

  variant
    vbin,
    vext,
    vstring,
    vcstring,
    varray,
    vmap;
  vbin = bin;
  vext = ext;
  vstring = str;
  vcstring = cstr;
  varray = arr;
  vmap = map;

  assert(vbin.type() == variant::Bin);
  assert(vext.type() == variant::Ext);
  assert(vstring.type() == variant::Str);
  assert(vcstring.type() == variant::Str);
  assert(varray.type() == variant::Array);
  assert(vmap.type() == variant::Map);
}

void testMove() {
  variant::binary bin { 1, 2, 3 };
  variant::extension ext { 8, { 1, 2, 3 } };
  variant::string str { "test" };
  variant::array arr { 1, 2, 3 };
  variant::map map { {1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'} };

  variant
    vbin,
    vext,
    vstring,
    varray,
    vmap;
  vbin = std::move(bin);
  vext = std::move(ext);
  vstring = std::move(str);
  varray = std::move(arr);
  vmap = std::move(map);

  assert(vbin.type() == variant::Bin);
  assert(vext.type() == variant::Ext);
  assert(vstring.type() == variant::Str);
  assert(varray.type() == variant::Array);
  assert(vmap.type() == variant::Map);
}

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  testCopy();
  testMove();

  return 0;
}
