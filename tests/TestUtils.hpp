#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include <vector>

#include <meiose/variant.hpp>

#define assert_exception(code, exception) try{code;assert((code, false));}catch(exception){}
#define assert_val_exception(code, exception) try{assert(code);}catch(exception){}
#define assert_float(from, to) assert(nearlyEqual(from.asFloat(), to));

template<typename T, typename U, typename V = decltype(T()*U())>
bool nearlyEqual(T a, U b, V epsilon = 0.00001f,
    V relth = std::numeric_limits<V>::min())
{
  assert(0.f < epsilon); assert(epsilon < 1.f);

  if (a == b) return true;

  auto diff = std::abs(a-b);
  auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<V>::max());
  return diff < epsilon * std::max(relth, norm);
}

void randomVariant(meiose::variant &v, std::mt19937 &r,
    std::vector<meiose::variant::Type> types, bool json, int depth) {
  using namespace meiose;
  using T = variant::Type;
  if (depth > 4) {
    types.erase(std::remove_if(types.begin(), types.end(), [](const variant::Type &t) -> bool {
      return t == T::Array || t == T::Map;
    }), types.end());
  }
  variant::Type type = types[std::uniform_int_distribution<size_t>(0, types.size() - 1)(r)];
  switch (type) {
  case variant::Nil:
    return;
  case variant::Bool:
    v = static_cast<bool>(std::uniform_int_distribution<int>(0, 1)(r));
    return;
  case variant::Bin: {
    v.type(variant::Type::Bin);
    variant::binary &b = v.getBin();
    const size_t sz = std::uniform_int_distribution<size_t>(4, 12)(r);
    b.reserve(sz);
    auto rbyte = std::uniform_int_distribution<uint8_t>();
    for (size_t i = 0; i < sz; ++i) {
      b.push_back(rbyte(r));
    }
  } return;
  case variant::Ext: {
    v.type(variant::Type::Ext);
    variant::extension &e = v.getExt();
    e.first = std::uniform_int_distribution<signed char>(0, 127)(r);
    const size_t sz = std::uniform_int_distribution<size_t>(4, 12)(r);
    e.second.reserve(sz);
    auto rbyte = std::uniform_int_distribution<uint8_t>();
    for (size_t i = 0; i < sz; ++i) {
      e.second.push_back(rbyte(r));
    }
  } return;
  case variant::Float:
    v = std::uniform_real_distribution<>(
        std::numeric_limits<double>::min(), std::numeric_limits<double>::max())(r);
    return;
  case variant::Int:
    v = std::uniform_int_distribution<int64_t>(
        std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max())(r);
    return;
  case variant::Str: {
    auto rchar = std::uniform_int_distribution<char>(' ', '~');
    v = std::string(std::uniform_int_distribution<size_t>(4, 24)(r), ' ');
    std::string &str = v.getStr();
    for (size_t i = 0; i < str.size(); ++i) {
      str[i] = rchar(r);
    }
  } return;
  case variant::Array: {
    v.type(variant::Type::Array);
    variant::array &a = v.getArray();
    const size_t sz = std::uniform_int_distribution<size_t>(4, 12)(r);
    a.reserve(sz);
    for (size_t i = 0; i < sz; ++i) {
      a.emplace_back();
      randomVariant(a.back(), r, types, json, depth + 1);
    }
  } return;
  case variant::Map:{
    v.type(variant::Type::Map);
    variant::map &m = v.getMap();
    size_t sz = std::uniform_int_distribution<size_t>(2, 8)(r);
    for (size_t i = 0; i < sz; ++i) {
      variant key;
      if (json) {
        randomVariant(key, r, { T::Str }, json, depth + 1);
      } else {
        randomVariant(key, r, types, json, depth + 1);
      }
      auto it = m.emplace(std::piecewise_construct,
          std::forward_as_tuple(std::move(key)),
          std::forward_as_tuple()).first;
      randomVariant(it->second, r, types, json, depth + 1);
    }
  } return;
  }
}

meiose::variant randomVariant(std::mt19937::result_type seed, bool json = false, std::vector<meiose::variant::Type> types = {}) {
  std::mt19937 e1(seed);
  using T = meiose::variant::Type;
  if (types.empty()) {
    if (json) {
      types = { T::Nil, T::Bool, T::Float, T::Int, T::Str, T::Array, T::Map };
    } else {
      types = { T::Nil, T::Bool, T::Bin, T::Ext, T::Float, T::Int, T::Str, T::Array, T::Map };
    }
  }
  meiose::variant v;
  randomVariant(v, e1, types, json, 0);
  return v;
}

#endif /* TEST_UTILS_HPP */
