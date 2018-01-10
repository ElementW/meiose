#include <assert.h>
#include <meiose/variant.hpp>

using namespace meiose;

void testCopy() {
  variant
    vbin(variant::binary { 1, 2, 3 }),
    vext(variant::extension { 8, { 1, 2, 3 } }),
    vstring(variant::string { "test" }),
    varray(variant::array { 1, 2, 3 }),
    vmap(variant::map { {1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'} });

  variant
    c_vbin(vbin),
    c_vext(vext),
    c_vstring(vstring),
    c_varray(varray),
    c_vmap(vmap);

  assert(c_vbin.type() == variant::Bin);
  assert(c_vext.type() == variant::Ext);
  assert(c_vstring.type() == variant::Str);
  assert(c_varray.type() == variant::Array);
  assert(c_vmap.type() == variant::Map);
}

void testMove() {
  variant
    vbin(variant::binary { 1, 2, 3 }),
    vext(variant::extension { 8, { 1, 2, 3 } }),
    vstring(variant::string { "test" }),
    varray(variant::array { 1, 2, 3 }),
    vmap(variant::map { {1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'} });

  variant
    c_vbin(std::move(vbin)),
    c_vext(std::move(vext)),
    c_vstring(std::move(vstring)),
    c_varray(std::move(varray)),
    c_vmap(std::move(vmap));

  assert(c_vbin.type() == variant::Bin);
  assert(c_vext.type() == variant::Ext);
  assert(c_vstring.type() == variant::Str);
  assert(c_varray.type() == variant::Array);
  assert(c_vmap.type() == variant::Map);
}

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  testCopy();
  testMove();

  return 0;
}
