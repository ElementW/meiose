#include <assert.h>
#include <meiose/variant.hpp>

using namespace meiose;

void testPrimitiveType() {
  /* Primitive value types, default constructed using variant::Type. */
  variant
    vdefault,
    nil(variant::Nil),
    vnullptr(nullptr),
    vbool(variant::Bool),
    vfloat(variant::Float),
    vint(variant::Int);

  assert(vdefault.type() == variant::Nil);
  assert(nil.type() == variant::Nil);
  assert(vnullptr.type() == variant::Nil);
  assert(vbool.type() == variant::Bool && vbool.operator bool() == false);
  assert(vfloat.type() == variant::Float && vfloat.operator double() == 0.0);
  assert(vint.type() == variant::Int && vfloat.operator int64_t() == 0);
}

void testPrimitivePrvalue() {
  /* Primitive value types, prvalue parameters. */
  variant
    pr_nil(nullptr),
    pr_btrue(true),
    pr_bfalse(false),
    pr_f32(13.37f),
    pr_f64(90.01),
    pr_u8 (static_cast<uint8_t>(1)),
    pr_u16(static_cast<uint16_t>(2)),
    pr_u32(static_cast<uint32_t>(3)),
    pr_i8 (static_cast<int8_t>(4)),
    pr_i16(static_cast<int16_t>(5)),
    pr_i32(static_cast<int32_t>(6)),
    pr_i64(static_cast<int64_t>(7));

  assert(pr_nil.type() == variant::Nil);
  assert(pr_btrue.type() == variant::Bool && pr_btrue.operator bool() == true);
  assert(pr_bfalse.type() == variant::Bool && pr_bfalse.operator bool() == false);
  assert(pr_f32.type() == variant::Float);
  assert(pr_f64.type() == variant::Float);
  assert(pr_u8.type()  == variant::Int);
  assert(pr_u16.type() == variant::Int);
  assert(pr_u32.type() == variant::Int);
  assert(pr_i8.type()  == variant::Int);
  assert(pr_i16.type() == variant::Int);
  assert(pr_i32.type() == variant::Int);
  assert(pr_i64.type() == variant::Int);
}

void testPrimitiveLvalue() {
  /* Primitive value types, lvalue parameters. */
  decltype(nullptr) c_nil = nullptr;
  bool c_true = true, c_false = false;
  float c_f32 = 13.37f;
  double c_f64 = 90.01;
  uint8_t  c_u8 = 1;
  uint16_t c_u16 = 2;
  uint32_t c_u32 = 3;
  int8_t   c_i8 = 4;
  int16_t  c_i16 = 5;
  int32_t  c_i32 = 6;
  int64_t  c_i64 = 7;

  variant
    l_nil(c_nil),
    l_btrue(c_true),
    l_bfalse(c_false),
    l_f32(c_f32),
    l_f64(c_f64),
    l_u8 (c_u8),
    l_u16(c_u16),
    l_u32(c_u32),
    l_i8 (c_i8),
    l_i16(c_i16),
    l_i32(c_i32),
    l_i64(c_i64);

  assert(l_nil.type() == variant::Nil);
  assert(l_btrue.type() == variant::Bool && l_btrue.operator bool() == true);
  assert(l_bfalse.type() == variant::Bool && l_bfalse.operator bool() == false);
  assert(l_f32.type() == variant::Float);
  assert(l_f64.type() == variant::Float);
  assert(l_u8.type()  == variant::Int);
  assert(l_u16.type() == variant::Int);
  assert(l_u32.type() == variant::Int);
  assert(l_i8.type()  == variant::Int);
  assert(l_i16.type() == variant::Int);
  assert(l_i32.type() == variant::Int);
  assert(l_i64.type() == variant::Int);
}

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  testPrimitiveType();
  testPrimitivePrvalue();
  testPrimitiveLvalue();

  return 0;
}
