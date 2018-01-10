#ifndef MEIOSE_VARIANT_HPP
#define MEIOSE_VARIANT_HPP

#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <map>
#include <utility>
#include <vector>

namespace meiose {

class variant final {
public:
  enum Type : int32_t {
    Nil,
    Bool,
    Bin,
    Ext,
    Float,
    Int,
    Str,
    Array,
    Map
  };

  using binary = std::vector<uint8_t>;
  using extension = std::pair<int8_t, binary>;
  using string = std::string;
  using array = std::vector<variant>;
  using map = std::map<variant, variant>;

private:
  union {
    bool v_b;
    binary v_bin;
    extension v_ext;
    double v_f;
    int64_t v_i;
    string v_str;
    array v_arr;
    map v_map;
  };
  Type m_type;

  void construct();
  void destroy();

public:
  void clear();

  constexpr variant() :
    v_i(0),
    m_type(Nil) {
  }

  ~variant() {
    destroy();
  }

  variant(const variant &v) :
    v_i(0) {
    operator=(v);
  }
  variant(variant &&v) :
    v_i(0) {
    operator=(std::move(v));
  }

  variant(Type t) : variant() {
    type(t);
  }

  constexpr variant(decltype(nullptr)) :
    v_i(0),
    m_type(Nil) {
  }

  variant(void*) = delete;

  constexpr variant(bool b) :
    v_b(b),
    m_type(Bool) {
  }

  variant(const binary &b) :
    m_type(Bin) {
    new (&v_bin) binary(b);
  }
  variant(binary &&b) :
    m_type(Bin) {
    new (&v_bin) binary(std::move(b));
  }

  variant(const extension &e) :
    m_type(Ext) {
    new (&v_ext) extension(e);
  }
  variant(extension &&e) :
    m_type(Ext) {
    new (&v_ext) extension(std::move(e));
  }

  constexpr variant(double d) :
    v_f(d),
    m_type(Float) {
  }
  constexpr variant(float f) : variant(static_cast<double>(f)) {}

  constexpr variant(int64_t i) :
    v_i(i),
    m_type(Int) {
  }
  constexpr variant(int32_t i) : variant(static_cast<int64_t>(i)) {}
  constexpr variant(int16_t i) : variant(static_cast<int64_t>(i)) {}
  constexpr variant(int8_t i) : variant(static_cast<int64_t>(i)) {}
  // constexpr variant(uint64_t) = delete;
  constexpr variant(uint32_t i) : variant(static_cast<int64_t>(i)) {}
  constexpr variant(uint16_t i) : variant(static_cast<int64_t>(i)) {}
  constexpr variant(uint8_t i) : variant(static_cast<int64_t>(i)) {}

  variant(const string &s) :
    m_type(Str) {
    new (&v_str) string(s);
  }
  variant(string &&s) :
    m_type(Str) {
    new (&v_str) string(std::move(s));
  }
  variant(const char *s) :
    m_type(Str) {
    new (&v_str) string(s);
  }

  variant(const array &a) :
    m_type(Array) {
    new (&v_arr) array(a);
  }
  variant(array &&a) :
    m_type(Array) {
    new (&v_arr) array(std::move(a));
  }

  variant(const map &m) :
    m_type(Map) {
    new (&v_map) map(m);
  }
  variant(map &&m) :
    m_type(Map) {
    new (&v_map) map(std::move(m));
  }


  /*
   * Move/copy assignement operators.
   */

  variant& operator=(const variant &v) {
    m_type = v.m_type;
    switch (m_type) {
    case Bool:
      v_b = v.v_b;
      break;
    case Bin:
      new (&v_bin) binary(v.v_bin);
      break;
    case Ext:
      new (&v_ext) extension(v.v_ext);
      break;
    case Float:
      v_f = v.v_f;
      break;
    case Int:
      v_i = v.v_i;
      break;
    case Str:
      new (&v_str) string(v.v_str);
      break;
    case Array:
      new (&v_arr) array(v.v_arr);
      break;
    case Map:
      new (&v_map) map(v.v_map);
      break;
    case Nil:
      break;
    }
    return *this;
  }
  variant& operator=(variant &&v) {
    m_type = v.m_type;
    switch (m_type) {
    case Bool:
      v_b = v.v_b;
      break;
    case Bin:
      new (&v_bin) binary(std::move(v.v_bin));
      v.v_bin.~vector();
      break;
    case Ext:
      new (&v_ext) extension(std::move(v.v_ext));
      v.v_ext.~pair();
      break;
    case Float:
      v_f = v.v_f;
      break;
    case Int:
      v_i = v.v_i;
      break;
    case Str:
      new (&v_str) string(std::move(v.v_str));
      v.v_str.~string();
      break;
    case Array:
      new (&v_arr) array(std::move(v.v_arr));
      v.v_arr.~vector();
      break;
    case Map:
      new (&v_map) map(std::move(v.v_map));
      v.v_map.~map();
      break;
    case Nil:
      break;
    }
    v.m_type = Nil;
    return *this;
  }


  /*
   * Value assignement operators.
   */

  inline variant& operator=(decltype(nullptr)) {
    type(Nil);
    return *this;
  }

  variant& operator=(void*) = delete;

  inline variant& operator=(bool b) {
    type(Bool);
    v_b = b;
    return *this;
  }

  inline variant& operator=(const binary &b) {
    type(Bin);
    v_bin = b;
    return *this;
  }
  inline variant& operator=(binary &&b) {
    type(Bin);
    v_bin = std::move(b);
    return *this;
  }

  inline variant& operator=(const extension &e) {
    type(Ext);
    v_ext = e;
    return *this;
  }
  inline variant& operator=(extension &&e) {
    type(Ext);
    v_ext = std::move(e);
    return *this;
  }

  inline variant& operator=(double d) {
    type(Float);
    v_f = d;
    return *this;
  }
  inline variant& operator=(float f) { return operator=(static_cast<double>(f)); }

  inline variant& operator=(int64_t i) {
    type(Int);
    v_i = i;
    return *this;
  }
  inline variant& operator=(int32_t i) { return operator=(static_cast<int64_t>(i)); }
  inline variant& operator=(int16_t i) { return operator=(static_cast<int64_t>(i)); }
  inline variant& operator=(int8_t i) { return operator=(static_cast<int64_t>(i)); }
  // inline variant& operator=(uint64_t) = delete;
  inline variant& operator=(uint32_t i) { return operator=(static_cast<int64_t>(i)); }
  inline variant& operator=(uint16_t i) { return operator=(static_cast<int64_t>(i)); }
  inline variant& operator=(uint8_t i) { return operator=(static_cast<int64_t>(i)); }

  inline variant& operator=(const string &s) {
    type(Str);
    v_str = s;
    return *this;
  }
  inline variant& operator=(string &&s) {
    type(Str);
    v_str = std::move(s);
    return *this;
  }
  inline variant& operator=(const char *s) {
    type(Str);
    v_str = s;
    return *this;
  }

  inline variant& operator=(const array &a) {
    type(Array);
    v_arr = a;
    return *this;
  }
  inline variant& operator=(array &&a) {
    type(Array);
    v_arr = std::move(a);
    return *this;
  }

  inline variant& operator=(const map &m) {
    type(Map);
    v_map = m;
    return *this;
  }
  inline variant& operator=(map &&m) {
    type(Map);
    v_map = std::move(m);
    return *this;
  }


  /*
   * Stored type information access / setting.
   */

  template<typename T>
  static constexpr Type typeOf();

  constexpr Type type() const noexcept {
    return m_type;
  }
  void type(Type);

  constexpr bool isNil()    const noexcept { return m_type == Nil; }
  constexpr bool isBool()   const noexcept { return m_type == Bool; }
  constexpr bool isBin()    const noexcept { return m_type == Bin; }
  constexpr bool isExt()    const noexcept { return m_type == Ext; }
  constexpr bool isFloat()  const noexcept { return m_type == Float; }
  constexpr bool isInt()    const noexcept { return m_type == Int; }
  constexpr bool isNumber() const noexcept { return m_type == Float or m_type == Int; }
  constexpr bool isStr()    const noexcept { return m_type == Str; }
  constexpr bool isArray()  const noexcept { return m_type == Array; }
  constexpr bool isMap()    const noexcept { return m_type == Map; }

  template<typename T>
  constexpr bool is() const noexcept;


  /*
   * Equality operator. Also checks for type equality, since this class is also used as map keys.
   */

  template<typename T>
  constexpr bool operator!=(T &&i) const noexcept { return not operator==(std::forward<T>(i)); }

  inline bool operator==(const variant &v) const {
    if (m_type != v.m_type) {
      return false;
    }
    switch (m_type) {
    case Bool:
      return v_b == v.v_b;
    case Bin:
      return v_bin == v.v_bin;
    case Ext:
      return v_ext == v.v_ext;
    case Float:
      return v_f == v.v_f;
    case Int:
      return v_i == v.v_i;
    case Str:
      return v_str == v.v_str;
    case Array:
      return v_arr == v.v_arr;
    case Map:
      return v_map == v.v_map;
    case Nil:
      return true;
    }
    return false;
  }

  constexpr bool operator==(decltype(nullptr)) const noexcept {
    return isNil();
  }

  constexpr bool operator==(bool b) const noexcept {
    return isBool() and v_b == b;
  }

  inline bool operator==(const binary &b) const {
    return isBin() and v_bin == b;
  }

  inline bool operator==(const extension &e) const {
    return isExt() and v_ext == e;
  }

  constexpr bool operator==(double d) const noexcept {
    return isFloat() and v_f == d;
  }
  constexpr bool operator==(float f) const noexcept { return operator==(static_cast<double>(f)); }

  constexpr bool operator==(int64_t i) const noexcept {
    return isInt() and v_i == i;
  }
  constexpr bool operator==(int32_t i) const noexcept { return operator==(static_cast<int64_t>(i)); }
  constexpr bool operator==(int16_t i) const noexcept { return operator==(static_cast<int64_t>(i)); }
  constexpr bool operator==(int8_t i) const noexcept { return operator==(static_cast<int64_t>(i)); }
  // bool operator==(uint64_t) = delete;
  constexpr bool operator==(uint32_t i) const noexcept { return operator==(static_cast<int64_t>(i)); }
  constexpr bool operator==(uint16_t i) const noexcept { return operator==(static_cast<int64_t>(i)); }
  constexpr bool operator==(uint8_t i) const noexcept { return operator==(static_cast<int64_t>(i)); }

  inline bool operator==(const string &s) const {
    return isStr() and v_str == s;
  }
  inline bool operator==(const char *s) const {
    return isStr() and v_str == s;
  }

  inline bool operator==(const array &a) const {
    return isArray() and v_arr == a;
  }

  inline bool operator==(const map &m) const {
    return isMap() and v_map == m;
  }


  /*
   * Comparison operators. Checks for type equality then compares values lexicogaphically.
   */

  template<typename T>
  constexpr bool operator<=(T &&i) const noexcept { return not operator>(std::forward<T>(i)); }
  template<typename T>
  constexpr bool operator>=(T &&i) const noexcept { return not operator<(std::forward<T>(i)); }

  constexpr bool operator<(const variant &v) const noexcept {
    if (m_type != v.m_type) {
      return m_type < v.m_type;
    }
    switch (m_type) {
    case Bool:
      return v_b < v.v_b;
    case Bin:
      return v_bin < v.v_bin;
    case Ext:
      return v_ext < v.v_ext;
    case Float:
      return v_f < v.v_f;
    case Int:
      return v_i < v.v_i;
    case Str:
      return v_str < v.v_str;
    case Array:
      return v_arr < v.v_arr;
    case Map:
      return v_map < v.v_map;
    default:
      break;
    }
    return false;
  }
  constexpr bool operator>(const variant &v) const noexcept {
    if (m_type != v.m_type) {
      return m_type > v.m_type;
    }
    switch (m_type) {
    case Bool:
      return v_b > v.v_b;
    case Bin:
      return v_bin > v.v_bin;
    case Ext:
      return v_ext > v.v_ext;
    case Float:
      return v_f > v.v_f;
    case Int:
      return v_i > v.v_i;
    case Str:
      return v_str > v.v_str;
    case Array:
      return v_arr > v.v_arr;
    case Map:
      return v_map > v.v_map;
    default:
      break;
    }
    return false;
  }

  constexpr bool operator<(bool b) const noexcept {
    return (m_type != Bool) ? m_type < Bool : (not v_b) and b;
  }
  constexpr bool operator>(bool b) const noexcept {
    return (m_type != Bool) ? m_type > Bool : v_b and (not b);
  }

  constexpr bool operator<(const binary &b) const noexcept {
    return (m_type != Bin) ? m_type < Bin : v_bin < b;
  }
  constexpr bool operator>(const binary &b) const noexcept {
    return (m_type != Bin) ? m_type > Bin : v_bin > b;
  }

  constexpr bool operator<(const extension &e) const noexcept {
    return (m_type != Ext) ? m_type < Ext : v_ext < e;
  }
  constexpr bool operator>(const extension &e) const noexcept {
    return (m_type != Ext) ? m_type > Ext : v_ext > e;
  }

  constexpr bool operator<(const string &s) const noexcept {
    return (m_type != Str) ? m_type < Str : v_str < s;
  }
  constexpr bool operator>(const string &s) const noexcept {
    return (m_type != Str) ? m_type > Str : v_str > s;
  }
  constexpr bool operator<(const char *s) const noexcept {
    return (m_type != Str) ? m_type < Str : v_str < s;
  }
  constexpr bool operator>(const char *s) const noexcept {
    return (m_type != Str) ? m_type > Str : v_str > s;
  }

  constexpr bool operator<(const array &a) const noexcept {
    return (m_type != Array) ? m_type < Array : v_arr < a;
  }
  constexpr bool operator>(const array &a) const noexcept {
    return (m_type != Array) ? m_type > Array : v_arr > a;
  }

  constexpr bool operator<(const map &m) const noexcept {
    return (m_type != Map) ? m_type < Map : v_map < m;
  }
  constexpr bool operator>(const map &m) const noexcept {
    return (m_type != Map) ? m_type > Map : v_map > m;
  }

  /*
   * Cast
   */

  template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
  T asNumber() const noexcept {
    switch (m_type) {
    case Float:
      return T(v_f);
    case Int:
      return T(v_i);
    default:
      break;
    }
    return T();
  }

  double  asFloat() const noexcept { return asNumber<double>(); }
  int64_t asInt()   const noexcept { return asNumber<int64_t>(); }

private:
  static const variant const_nil;
  static const binary const_bin;
  static const extension const_ext;
  static const string const_str;
  static const array const_arr;
  static const map const_map;

public:
  operator bool() const noexcept {
    return isBool() ? v_b : false;
  }
  operator const binary&() const noexcept {
    return isBin() ? v_bin : const_bin;
  }
  const binary& getBin() const {
    if (not isBin()) throw std::runtime_error("Not a Bin");
    return v_bin;
  }
  binary& getBin() {
    if (not isBin()) throw std::runtime_error("Not a Bin");
    return v_bin;
  }
  operator const extension&() const noexcept {
    return isExt() ? v_ext : const_ext;
  }
  const extension& getExt() const {
    if (not isExt()) throw std::runtime_error("Not an Ext");
    return v_ext;
  }
  extension& getExt() {
    if (not isExt()) throw std::runtime_error("Not an Ext");
    return v_ext;
  }
  operator double() const noexcept {
    return asNumber<double>();
  }
  operator uint8_t() const noexcept {
    return asNumber<uint8_t>();
  }
  operator uint16_t() const noexcept {
    return asNumber<uint16_t>();
  }
  operator uint32_t() const noexcept {
    return asNumber<uint32_t>();
  }
  operator int8_t() const noexcept {
    return asNumber<int8_t>();
  }
  operator int16_t() const noexcept {
    return asNumber<int16_t>();
  }
  operator int32_t() const noexcept {
    return asNumber<int32_t>();
  }
  operator int64_t() const noexcept {
    return asNumber<int64_t>();
  }
  operator const string&() const noexcept {
    return isStr() ? v_str : const_str;
  }
  const string& getStr() const {
    if (not isStr()) throw std::runtime_error("Not a Str");
    return v_str;
  }
  string& getStr() {
    if (not isStr()) throw std::runtime_error("Not a Str");
    return v_str;
  }
  operator const array&() const noexcept {
    return isArray() ? v_arr : const_arr;
  }
  const array& getArray() const {
    if (not isArray()) throw std::runtime_error("Not an Array");
    return v_arr;
  }
  array& getArray() {
    if (not isArray()) throw std::runtime_error("Not an Array");
    return v_arr;
  }
  operator const map&() const noexcept {
    return isMap() ? v_map : const_map;
  }
  const map& getMap() const {
    if (not isMap()) throw std::runtime_error("Not a Map");
    return v_map;
  }
  map& getMap() {
    if (not isMap()) throw std::runtime_error("Not a Map");
    return v_map;
  }


  /*
   * Helper methods.
   */

  void put(const variant &key, const variant &val) {
    if (isMap()) {
      v_map.emplace(std::piecewise_construct,
                    std::forward_as_tuple(key), std::forward_as_tuple(val));
      return;
    }
    throw std::runtime_error("Not a map");
  }

  variant& operator[](uint8_t idx) { return operator[](static_cast<uint32_t>(idx)); }
  variant& operator[](uint16_t idx) { return operator[](static_cast<uint32_t>(idx)); }
  variant& operator[](uint32_t idx) {
    if (isArray()) {
      return v_arr.at(idx);
    } else if (isMap()) {
      return v_map.at(idx);
    }
    throw std::runtime_error("Not an array or map");
  }
  const variant& operator[](uint8_t idx) const { return operator[](static_cast<uint32_t>(idx)); }
  const variant& operator[](uint16_t idx) const { return operator[](static_cast<uint32_t>(idx)); }
  const variant& operator[](uint32_t idx) const {
    if (isArray()) {
      return v_arr.at(idx);
    } else if (isMap()) {
      return v_map.at(idx);
    }
    throw std::runtime_error("Not an array or map");
  }
  variant& operator[](int8_t idx) { return operator[](static_cast<int64_t>(idx)); }
  variant& operator[](int16_t idx) { return operator[](static_cast<int64_t>(idx)); }
  variant& operator[](int32_t idx) { return operator[](static_cast<int64_t>(idx)); }
  variant& operator[](int64_t idx) {
    if (isArray()) {
      if (idx < 0) {
        throw std::runtime_error("Negative key");
      }
      return v_arr.at(idx);
    } else if (isMap()) {
      return v_map.at(idx);
    }
    throw std::runtime_error("Not an array or map");
  }
  const variant& operator[](int8_t idx) const { return operator[](static_cast<int64_t>(idx)); }
  const variant& operator[](int16_t idx) const { return operator[](static_cast<int64_t>(idx)); }
  const variant& operator[](int32_t idx) const { return operator[](static_cast<int64_t>(idx)); }
  const variant& operator[](int64_t idx) const {
    if (isArray()) {
      if (idx < 0) {
        throw std::runtime_error("Negative key");
      }
      return v_arr.at(idx);
    } else if (isMap()) {
      return v_map.at(idx);
    }
    throw std::runtime_error("Not an array or map");
  }
  variant& operator[](decltype(nullptr)) { return operator[](variant()); }
  variant& operator[](const char *key) { return operator[](variant(key)); }
  variant& operator[](const variant &key) {
    if (isArray()) {
      if (not key.isInt()) {
        throw std::runtime_error("Non-integer key");
      } else if (key.v_i < 0) {
        throw std::runtime_error("Negative key");
      }
      return v_arr.at(key.v_i);
    } else if (isMap()) {
      return v_map.at(key);
    }
    throw std::runtime_error("Not an array or map");
  }
  const variant& operator[](decltype(nullptr)) const { return operator[](variant()); }
  const variant& operator[](const char *key) const { return operator[](variant(key)); }
  const variant& operator[](const variant &key) const {
    if (isArray()) {
      if (not key.isInt()) {
        throw std::runtime_error("Non-integer key");
      } else if (key.v_i < 0) {
        throw std::runtime_error("Negative key");
      }
      return v_arr.at(key.v_i);
    } else if (isMap()) {
      return v_map.at(key);
    }
    throw std::runtime_error("Not an array or map");
  }

  size_t size() const noexcept {
    switch (m_type) {
    case Bin:
      return v_bin.size();
    case Ext:
      return v_ext.second.size();
    case Str:
      return v_str.size();
    case Array:
      return v_arr.size();
    case Map:
      return v_map.size();
    default:
      break;
    }
    return 0;
  }

  void push(const variant &v) {
    if (not isArray()) {
      throw std::runtime_error("Not an array");
    }
    v_arr.emplace_back(v);
  }
  void push(variant &&v) {
    if (not isArray()) {
      throw std::runtime_error("Not an array");
    }
    v_arr.emplace_back(std::move(v));
  }
};

template<>
constexpr variant::Type variant::typeOf<decltype(nullptr)>() {
  return Nil;
}
template<>
constexpr variant::Type variant::typeOf<bool>() {
  return Bool;
}
template<>
constexpr variant::Type variant::typeOf<variant::binary>() {
  return Bin;
}
template<>
constexpr variant::Type variant::typeOf<variant::extension>() {
  return Ext;
}
template<>
constexpr variant::Type variant::typeOf<float>() {
  return Float;
}
template<>
constexpr variant::Type variant::typeOf<double>() {
  return Float;
}
template<>
constexpr variant::Type variant::typeOf<uint8_t>() {
  return Int;
}
template<>
constexpr variant::Type variant::typeOf<uint16_t>() {
  return Int;
}
template<>
constexpr variant::Type variant::typeOf<uint32_t>() {
  return Int;
}
/* template<>
constexpr variant::Type variant::typeOf<uint64_t>() {
  return Int;
} */
template<>
constexpr variant::Type variant::typeOf<int8_t>() {
  return Int;
}
template<>
constexpr variant::Type variant::typeOf<int16_t>() {
  return Int;
}
template<>
constexpr variant::Type variant::typeOf<int32_t>() {
  return Int;
}
template<>
constexpr variant::Type variant::typeOf<int64_t>() {
  return Int;
}
template<>
constexpr variant::Type variant::typeOf<variant::string>() {
  return Str;
}
template<>
constexpr variant::Type variant::typeOf<variant::array>() {
  return Array;
}
template<>
constexpr variant::Type variant::typeOf<variant::map>() {
  return Map;
}

template<>
constexpr bool variant::is<decltype(nullptr)>() const noexcept {
  return isNil();
}
template<>
constexpr bool variant::is<bool>() const noexcept {
  return isBool();
}
template<>
constexpr bool variant::is<variant::binary>() const noexcept {
  return isBin();
}
template<>
constexpr bool variant::is<variant::extension>() const noexcept {
  return isExt();
}
template<>
constexpr bool variant::is<float>() const noexcept {
  return isFloat();
}
template<>
constexpr bool variant::is<double>() const noexcept {
  return isFloat();
}
template<>
constexpr bool variant::is<uint8_t>() const noexcept {
  return isInt();
}
template<>
constexpr bool variant::is<uint16_t>() const noexcept {
  return isInt();
}
template<>
constexpr bool variant::is<uint32_t>() const noexcept {
  return isInt();
}
/* template<>
constexpr bool variant::is<uint64_t>() const noexcept {
  return isInt();
} */
template<>
constexpr bool variant::is<int8_t>() const noexcept {
  return isInt();
}
template<>
constexpr bool variant::is<int16_t>() const noexcept {
  return isInt();
}
template<>
constexpr bool variant::is<int32_t>() const noexcept {
  return isInt();
}
template<>
constexpr bool variant::is<int64_t>() const noexcept {
  return isInt();
}
template<>
constexpr bool variant::is<variant::string>() const noexcept {
  return isStr();
}
template<>
constexpr bool variant::is<variant::array>() const noexcept {
  return isArray();
}
template<>
constexpr bool variant::is<variant::map>() const noexcept {
  return isMap();
}

using binary = variant::binary;
using extension = variant::extension;
using string = variant::string;
using array = variant::array;
using map = variant::map;

}

#endif /* MEIOSE_VARIANT_HPP */
