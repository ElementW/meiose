#include <assert.h>
#include <meiose/variant.hpp>

using namespace meiose;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  variant
    nil,
    btrue,
    bfalse,
    f32,
    f64,
    u8,
    u16,
    u32,
    i8,
    i16,
    i32,
    i64;

  nil = nullptr;
  btrue = true;
  bfalse = false;
  f32 = 13.37f;
  f64 = 90.01;
  u8  = static_cast<uint8_t>(1);
  u16 = static_cast<uint16_t>(2);
  u32 = static_cast<uint32_t>(3);
  i8  = static_cast<int8_t>(4);
  i16 = static_cast<int16_t>(5);
  i32 = static_cast<int32_t>(6);
  i64 = static_cast<int64_t>(7);

  assert(nil.type() == variant::Nil);
  assert(btrue.type() == variant::Bool);
  assert(bfalse.type() == variant::Bool);
  assert(f32.type() == variant::Float);
  assert(f64.type() == variant::Float);
  assert(u8.type()  == variant::Int);
  assert(u16.type() == variant::Int);
  assert(u32.type() == variant::Int);
  assert(i8.type()  == variant::Int);
  assert(i16.type() == variant::Int);
  assert(i32.type() == variant::Int);
  assert(i64.type() == variant::Int);

  return 0;
}
