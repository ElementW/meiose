#include <assert.h>
#include <meiose/variant.hpp>

using namespace meiose;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  assert(variant::typeOf<decltype(nullptr)>() == variant::Nil);
  assert(variant::typeOf<bool>() == variant::Bool);
  assert(variant::typeOf<variant::binary>() == variant::Bin);
  assert(variant::typeOf<variant::extension>() == variant::Ext);
  assert(variant::typeOf<float>() == variant::Float);
  assert(variant::typeOf<double>() == variant::Float);
  assert(variant::typeOf<uint8_t>() == variant::Int);
  assert(variant::typeOf<uint16_t>() == variant::Int);
  assert(variant::typeOf<uint32_t>() == variant::Int);
  assert(variant::typeOf<int8_t>() == variant::Int);
  assert(variant::typeOf<int16_t>() == variant::Int);
  assert(variant::typeOf<int32_t>() == variant::Int);
  assert(variant::typeOf<int64_t>() == variant::Int);
  assert(variant::typeOf<variant::string>() == variant::Str);
  assert(variant::typeOf<variant::array>() == variant::Array);
  assert(variant::typeOf<variant::map>() == variant::Map);

  variant
    vdefault,
    nil(variant::Nil),
    vnullptr(nullptr),
    vbool(variant::Bool),
    vfloat(variant::Float),
    vint(variant::Int),
    vbin(variant::Bin),
    vext(variant::Ext),
    vstring(variant::Str),
    varray(variant::Array),
    vmap(variant::Map);

  assert(vdefault.isNil());
  assert(nil.isNil());
  assert(vnullptr.isNil());
  assert(vbool.isBool());
  assert(vfloat.isFloat());
  assert(vint.isInt());
  assert(vfloat.isNumber());
  assert(vint.isNumber());
  assert(vbin.isBin());
  assert(vext.isExt());
  assert(vstring.isStr());
  assert(varray.isArray());
  assert(vmap.isMap());

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
    assert(vars[i].is<decltype(nullptr)>() == (i == 0));

    assert(vars[i].is<bool>() == (i == 1 || i == 2));

    assert(vars[i].is<variant::binary>() == (i == 12));

    assert(vars[i].is<variant::extension>() == (i == 13));

    assert(vars[i].is<float>() == (i == 3 || i == 4));
    assert(vars[i].is<double>() == (i == 3 || i == 4));

    assert(vars[i].is<uint8_t>() == (i >= 5 && i <= 11));
    assert(vars[i].is<uint16_t>() == (i >= 5 && i <= 11));
    assert(vars[i].is<uint32_t>() == (i >= 5 && i <= 11));
    assert(vars[i].is<int8_t>() == (i >= 5 && i <= 11));
    assert(vars[i].is<int16_t>() == (i >= 5 && i <= 11));
    assert(vars[i].is<int32_t>() == (i >= 5 && i <= 11));
    assert(vars[i].is<int64_t>() == (i >= 5 && i <= 11));

    assert(vars[i].is<variant::string>() == (i == 14));

    assert(vars[i].is<variant::array>() == (i == 15));

    assert(vars[i].is<variant::map>() == (i == 16));
  }

  return 0;
}
