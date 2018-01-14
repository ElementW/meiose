/* This file is part of meiose.
 *
 *meiose is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *meiose is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with meiose.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <meiose/msgpack.hpp>

#include <meiose/endian.hpp>
#include <meiose/exceptions.hpp>

namespace meiose {
namespace msgpack {

using namespace endian;

static void readMap(std::istream &iss, variant &v, size_t count) {
  v.type(variant::Map);
  for (size_t i = 0; i < count; ++i) {
    variant key;
    read(iss, key);
    auto it = v.getMap().emplace(std::piecewise_construct,
        std::forward_as_tuple(std::move(key)),
        std::forward_as_tuple());
    read(iss, it.first->second);
  }
}

static void readArray(std::istream &iss, variant &v, size_t count) {
  v.type(variant::Array);
  variant::array &arr = v.getArray();
  arr.resize(count);
  for (size_t i = 0; i < count; ++i) {
    read(iss, arr[i]);
  }
}

static void readString(std::istream &iss, variant &v, size_t size) {
  v.type(variant::Str);
  v.getStr().resize(size, '\0');
  iss.read(&v.getStr()[0], static_cast<std::streamsize>(size));
}

static void readBin(std::istream &iss, variant &v, size_t size) {
  v.type(variant::Bin);
  v.getBin().resize(size, 0);
  iss.read(reinterpret_cast<char*>(&v.getBin()[0]), static_cast<std::streamsize>(size));
}

static void readExt(std::istream &iss, variant &v, size_t size) {
  v.type(variant::Ext);
  v.getExt().second.resize(size, 0);
  v.getExt().first = static_cast<int8_t>(iss.get());
  iss.read(reinterpret_cast<char*>(&v.getExt().second[0]), static_cast<std::streamsize>(size));
}

template<typename T, typename U>
static inline T bitcast(U u) {
  union { U from; T to; } b = { u };
  return b.to;
}

void read(std::istream &iss, variant &v) {
  struct ISSException {
    // RAII-style "finally" handler to restore the input stream's exception settings
    std::istream &iss;
    std::ios_base::iostate exc;
    ISSException(std::istream &iss) : iss(iss) {
      exc = iss.exceptions();
    }
    ~ISSException() {
      iss.exceptions(exc);
    }
  } issException(iss);
  using ios = std::ios_base;
  iss.exceptions(ios::badbit | ios::failbit | ios::eofbit);
  auto read8 = [&iss]() -> uint8_t { return static_cast<uint8_t>(iss.get()); };
  auto read16 = [&iss]() -> uint16_t {
    union { uint16_t i; char b[2]; } u;
    iss.read(u.b, 2);
    return fromBe16(u.i);
  };
  auto read32 = [&iss]() -> uint32_t {
    union { uint32_t i; char b[4]; } u;
    iss.read(u.b, 4);
    return fromBe32(u.i);
  };
  auto read64 = [&iss]() -> uint64_t {
    union { uint64_t i; char b[8]; } u;
    iss.read(u.b, 8);
    return fromBe64(u.i);
  };
  try {
    uint8_t type = read8();
    if (type <= 0x7F) {
      v = type;
    } else if (type <= 0x8F) {
      readMap(iss, v, type & 0xF);
    } else if (type <= 0x9F) {
      readArray(iss, v, type & 0xF);
    } else if (type <= 0xBF) {
      readString(iss, v, type & 0x1F);
    } else if (type <= 0xDF) {
      switch (type) {
      case 0xC0:
        v = nullptr;
        break;
      case 0xC1:
        throw invalid_input("Invalid MessagePack type 0xC1");
      case 0xC2:
        v = false;
        break;
      case 0xC3:
        v = true;
        break;
      case 0xC4:
        readBin(iss, v, read8());
        break;
      case 0xC5:
        readBin(iss, v, read16());
        break;
      case 0xC6:
        readBin(iss, v, read32());
        break;
      case 0xC7:
        readExt(iss, v, read8());
        break;
      case 0xC8:
        readExt(iss, v, read16());
        break;
      case 0xC9:
        readExt(iss, v, read32());
        break;
      case 0xCA:
        v = bitcast<float>(read32());
        break;
      case 0xCB:
        v = bitcast<double>(read64());
        break;
      case 0xCC:
        v = read8();
        break;
      case 0xCD:
        v = read16();
        break;
      case 0xCE:
        v = read32();
        break;
      case 0xCF:
        v = bitcast<int64_t>(read64());
        break;
      case 0xD0:
        v = bitcast<int8_t>(read8());
        break;
      case 0xD1:
        v = bitcast<int16_t>(read16());
        break;
      case 0xD2:
        v = bitcast<int32_t>(read32());
        break;
      case 0xD3:
        v = bitcast<int64_t>(read64());
        break;
      case 0xD4:
        readExt(iss, v, 1);
        break;
      case 0xD5:
        readExt(iss, v, 2);
        break;
      case 0xD6:
        readExt(iss, v, 4);
        break;
      case 0xD7:
        readExt(iss, v, 8);
        break;
      case 0xD8:
        readExt(iss, v, 16);
        break;
      case 0xD9:
        readString(iss, v, read8());
        break;
      case 0xDA:
        readString(iss, v, read16());
        break;
      case 0xDB:
        readString(iss, v, read32());
        break;
      case 0xDC:
        readArray(iss, v, read16());
        break;
      case 0xDD:
        readArray(iss, v, read32());
        break;
      case 0xDE:
        readMap(iss, v, read16());
        break;
      case 0xDF:
        readMap(iss, v, read32());
        break;
      }
    } else {
      v = -(type & 0x1F);
    }
  } catch (std::istringstream::failure &e) {
    if (iss.eof()) {
      throw unexpected_eof("EOF while reading MessagePack");
    }
    throw e;
  }
}

void write(std::ostream &oss, const variant &v) {
  auto put = [&oss](auto c) { oss.put(static_cast<char>(c)); };
  auto put16 = [&oss](auto p) {
    static_assert(sizeof(p) == 2, "p isn't 2 bytes long");
    union { decltype(p) d; uint16_t i; char b[2]; } u{p};
    u.i = toBe16(u.i);
    oss.write(u.b, 2);
  };
  auto put32 = [&oss](auto p) {
    static_assert(sizeof(p) == 4, "p isn't 4 bytes long");
    union { decltype(p) d; uint32_t i; char b[4]; } u{p};
    u.i = toBe32(u.i);
    oss.write(u.b, 4);
  };
  auto put64 = [&oss](auto p) {
    static_assert(sizeof(p) == 8, "p isn't 8 bytes long");
    union { decltype(p) d; uint64_t i; char b[8]; } u{p};
    u.i = toBe64(u.i);
    oss.write(u.b, 8);
  };
  switch (v.type()) {
  case variant::Nil:
    put(0xC0);
    break;
  case variant::Bool:
    put(v ? 0xC3 : 0xC2);
    break;
  case variant::Bin: {
    size_t s = v.size();
    if (s <= 0xFF) {
      put(0xC4);
      put(static_cast<uint8_t>(s));
    } else if (s <= 0xFFFF) {
      put(0xC5);
      put16(static_cast<uint16_t>(s));
    } else {
      put(0xC6);
      put32(static_cast<uint32_t>(s));
    }
    oss.write(reinterpret_cast<const char*>(static_cast<const variant::binary&>(v).data()), s);
  } break;
  case variant::Ext: {
    auto &ext = v.operator const variant::extension&();
    size_t s = ext.second.size();
    if (s <= 0xFF) {
      if (s == 1) {
        put(0xD4);
      } else if (s == 2) {
        put(0xD5);
      } else if (s == 4) {
        put(0xD6);
      } else if (s == 8) {
        put(0xD7);
      } else if (s == 16) {
        put(0xD8);
      } else {
        put(0xC7);
        put(static_cast<uint8_t>(s));
      }
    } else if (s <= 0xFFFF) {
      put(0xC8);
      put16(static_cast<uint16_t>(s));
    } else {
      put(0xC9);
      put32(static_cast<uint32_t>(s));
    }
    put(ext.first);
    oss.write(reinterpret_cast<const char*>(ext.second.data()), s);
  } break;
  case variant::Float: {
    double f = v;
    put(0xCB);
    put64(f);
  } break;
  case variant::Int: {
    int64_t i = v;
    if (i >= 0) {
      if (i < 2 << 7) {
        put(i);
      } else if (i < 2 << 8) {
        put(0xCC);
        put(i);
      } else if (i < 2 << 16) {
        put(0xCD);
        put16(static_cast<uint16_t>(i));
      } else if (i < 2l << 32) {
        put(0xCE);
        put32(static_cast<uint32_t>(i));
      } else {
        put(0xCF);
        put64(static_cast<uint64_t>(i));
      }
    } else {
      if (-i < 2 << 5) {
        put(0b11100000 | -i);
      } else if (-i < 2 << 8) {
        put(0xD0);
        put(static_cast<char>(i));
      } else if (-i < 2 << 16) {
        put(0xD1);
        put16(static_cast<int16_t>(i));
      } else if (-i < 2l << 32) {
        put(0xD2);
        put32(static_cast<int32_t>(i));
      } else {
        put(0xD3);
        put64(i);
      }
    }
  } break;
  case variant::Str: {
    const std::string &s = v;
    if (s.size() <= 31) {
      put(0b10100000 | s.size());
    } else if (s.size() < 2 << 8) {
      put(0xD9);
      put(s.size());
    } else if (s.size() < 2 << 16) {
      put(0xDA);
      put16(static_cast<uint16_t>(s.size()));
    } else if (s.size() < 2l << 32) {
      put(0xDB);
      put32(static_cast<uint32_t>(s.size()));
    } else {
      throw std::runtime_error("String too long");
    }
    oss << s;
  } break;
  case variant::Array: {
    auto &a = v.operator const array&();
    size_t s = a.size();
    if (s <= 15) {
      put(0b10010000 | s);
    } else if (s < 2 << 16) {
      put(0xDC);
      put16(static_cast<uint16_t>(s));
    } else if (s < 2l << 32) {
      put(0xDD);
      put32(static_cast<uint32_t>(s));
    } else {
      throw std::runtime_error("Array too long");
    }
    for (const variant &v : a) {
      write(oss, v);
    }
  } break;
  case variant::Map: {
    auto &m = v.operator const map&();
    size_t s = m.size();
    if (s <= 15) {
      put(0b10000000 | s);
    } else if (s < 2 << 16) {
      put(0xDE);
      put16(static_cast<uint16_t>(s));
    } else if (s < 2l << 32) {
      put(0xDF);
      put32(static_cast<uint32_t>(s));
    } else {
      throw std::runtime_error("Map too long");
    }
    for (const std::pair<const variant, variant> &p : m) {
      write(oss, p.first);
      write(oss, p.second);
    }
  } break;
  }
}

}
}
