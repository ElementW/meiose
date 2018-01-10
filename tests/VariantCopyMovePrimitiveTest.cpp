#include <assert.h>
#include <meiose/variant.hpp>

using namespace meiose;

void testCopy() {
  variant
    nil(nullptr),
    btrue(true),
    bfalse(false),
    f32(13.37f),
    f64(90.01),
    u8 (static_cast<uint8_t>(1)),
    u16(static_cast<uint16_t>(2)),
    u32(static_cast<uint32_t>(3)),
    i8 (static_cast<int8_t>(4)),
    i16(static_cast<int16_t>(5)),
    i32(static_cast<int32_t>(6)),
    i64(static_cast<int64_t>(7));

  variant
    c_nil(nil),
    c_btrue(btrue),
    c_bfalse(bfalse),
    c_f32(f32),
    c_f64(f64),
    c_u8 (u8),
    c_u16(u16),
    c_u32(u32),
    c_i8 (i8),
    c_i16(i16),
    c_i32(i32),
    c_i64(i64);

  assert(c_nil.type() == variant::Nil);
  assert(c_btrue.type() == variant::Bool);
  assert(c_bfalse.type() == variant::Bool);
  assert(c_f32.type() == variant::Float);
  assert(c_f64.type() == variant::Float);
  assert(c_u8.type()  == variant::Int);
  assert(c_u16.type() == variant::Int);
  assert(c_u32.type() == variant::Int);
  assert(c_i8.type()  == variant::Int);
  assert(c_i16.type() == variant::Int);
  assert(c_i32.type() == variant::Int);
  assert(c_i64.type() == variant::Int);
}

void testMove() {
  variant
    nil(nullptr),
    btrue(true),
    bfalse(false),
    f32(13.37f),
    f64(90.01),
    u8 (static_cast<uint8_t>(1)),
    u16(static_cast<uint16_t>(2)),
    u32(static_cast<uint32_t>(3)),
    i8 (static_cast<int8_t>(4)),
    i16(static_cast<int16_t>(5)),
    i32(static_cast<int32_t>(6)),
    i64(static_cast<int64_t>(7));

  variant
    m_nil(std::move(nil)),
    m_btrue(std::move(btrue)),
    m_bfalse(std::move(bfalse)),
    m_f32(std::move(f32)),
    m_f64(std::move(f64)),
    m_u8 (std::move(u8)),
    m_u16(std::move(u16)),
    m_u32(std::move(u32)),
    m_i8 (std::move(i8)),
    m_i16(std::move(i16)),
    m_i32(std::move(i32)),
    m_i64(std::move(i64));

  assert(m_nil.type() == variant::Nil);
  assert(m_btrue.type() == variant::Bool);
  assert(btrue.type() == variant::Nil);
  assert(m_bfalse.type() == variant::Bool);
  assert(bfalse.type() == variant::Nil);
  assert(m_f32.type() == variant::Float);
  assert(f32.type() == variant::Nil);
  assert(m_f64.type() == variant::Float);
  assert(f64.type() == variant::Nil);
  assert(m_u8.type()  == variant::Int);
  assert(u8.type() == variant::Nil);
  assert(m_u16.type() == variant::Int);
  assert(u16.type() == variant::Nil);
  assert(m_u32.type() == variant::Int);
  assert(u32.type() == variant::Nil);
  assert(m_i8.type()  == variant::Int);
  assert(i8.type() == variant::Nil);
  assert(m_i16.type() == variant::Int);
  assert(i16.type() == variant::Nil);
  assert(m_i32.type() == variant::Int);
  assert(i32.type() == variant::Nil);
  assert(m_i64.type() == variant::Int);
  assert(i64.type() == variant::Nil);
}

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  testCopy();
  testMove();

  return 0;
}
