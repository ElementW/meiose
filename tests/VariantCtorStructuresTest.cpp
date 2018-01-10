#include <assert.h>
#include <meiose/variant.hpp>

using namespace meiose;

void testStructureType() {
  /* Structure value types, default constructed using variant::Type. */
  variant
    vbin(variant::Bin),
    vext(variant::Ext),
    vstring(variant::Str),
    varray(variant::Array),
    vmap(variant::Map);

  assert(vbin.type() == variant::Bin && vbin.operator const binary&() == variant::binary());
  assert(vext.type() == variant::Ext && vext.operator const extension&() == variant::extension());
  assert(vstring.type() == variant::Str && vstring.operator const string&() == variant::string());
  assert(varray.type() == variant::Array && varray.operator const array&() == variant::array());
  assert(vmap.type() == variant::Map && vmap.operator const map&() == variant::map());
}

void testStructureCopy() {
  /* Structure value types, const reference parameters. */
  const variant::binary bin;
  const variant::extension ext;
  const variant::string str;
  const char *cstr = "";
  const variant::array arr;
  const variant::map map;
  variant
    vbin(bin),
    vext(ext),
    vcstring(cstr),
    vstring(str),
    varray(arr),
    vmap(map);

  assert(vbin.type() == variant::Bin);
  assert(vext.type() == variant::Ext);
  assert(vstring.type() == variant::Str);
  assert(vcstring.type() == variant::Str);
  assert(varray.type() == variant::Array);
  assert(vmap.type() == variant::Map);
}

void testStructureMove() {
  /* Structure value types, moved parameters. */
  variant::binary bin;
  variant::extension ext;
  variant::string str;
  variant::array arr;
  variant::map map;
  variant
    vbin(std::move(bin)),
    vext(std::move(ext)),
    vstring(std::move(str)),
    varray(std::move(arr)),
    vmap(std::move(map));

  assert(vbin.type() == variant::Bin);
  assert(vext.type() == variant::Ext);
  assert(vstring.type() == variant::Str);
  assert(varray.type() == variant::Array);
  assert(vmap.type() == variant::Map);
}

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  testStructureType();
  testStructureCopy();
  testStructureMove();

  return 0;
}
