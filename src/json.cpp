#include <meiose/json.hpp>

#include <cstdlib>
#include <cstring>

namespace meiose {
namespace json {

static const char *escapechars[] = {
  "\\u0000",
  "\\u0001",
  "\\u0002",
  "\\u0003",
  "\\u0004",
  "\\u0005",
  "\\u0006",
  "\\u0007",
  "\\b",
  "\\t",
  "\\n",
  "\\u000B",
  "\\f",
  "\\r",
  "\\u000E",
  "\\u000F",
  "\\u0010",
  "\\u0011",
  "\\u0012",
  "\\u0013",
  "\\u0014",
  "\\u0015",
  "\\u0016",
  "\\u0017",
  "\\u0018",
  "\\u0019",
  "\\u001A",
  "\\u001B",
  "\\u001C",
  "\\u001D",
  "\\u001E",
  "\\u001F"
};

static const char *xescapechars[] = {
  "\\x00",
  "\\x01",
  "\\x02",
  "\\x03",
  "\\x04",
  "\\x05",
  "\\x06",
  "\\x07",
  "\\b",
  "\\t",
  "\\n",
  "\\x0B",
  "\\f",
  "\\r",
  "\\x0E",
  "\\x0F",
  "\\x10",
  "\\x11",
  "\\x12",
  "\\x13",
  "\\x14",
  "\\x15",
  "\\x16",
  "\\x17",
  "\\x18",
  "\\x19",
  "\\x1A",
  "\\x1B",
  "\\x1C",
  "\\x1D",
  "\\x1E",
  "\\x1F"
};

void write(std::ostringstream &oss, const variant &v, WriteFlags wf) {
  switch (v.type()) {
  case variant::Nil:
    oss << "null";
    break;
  case variant::Bool:
    oss << (v ? "true" : "false");
    break;
  case variant::Bin:
    throw not_encodable("Cannot encode Bin type into JSON");
  case variant::Ext:
    throw not_encodable("Cannot encode Ext type into JSON");
  case variant::Float:
    oss << static_cast<double>(v);
    break;
  case variant::Int:
    oss << static_cast<int64_t>(v);
    break;
  case variant::Str: {
    const std::string &s = v;
    oss << '"';
    for (char c : s) {
      if (c == '"') {
        oss << "\\\"";
      } else if (c == '\\') {
        oss << "\\\\";
      } else if (c > '\0' && c <= '\x1F') {
        // Control characters, as described by ECMA-404 chapter 9
        if (wf & ByteXEscape) {
          oss << xescapechars[static_cast<int>(c)];
        } else {
          oss << escapechars[static_cast<int>(c)];
        }
      } else {
        oss << c;
      }
    }
    oss << '"';
  } break;
  case variant::Array: {
    oss << '[';
    bool first = true;
    for (auto &elm : v.operator const variant::array&()) {
      if (first) {
        first = false;
      } else {
        oss << ',';
      }
      write(oss, elm);
    }
    oss << ']';
  } break;
  case variant::Map: {
    oss << '{';
    bool first = true;
    for (auto &pair : v.operator const variant::map&()) {
      if (first) {
        first = false;
      } else {
        oss << ',';
      }
      if (!(wf & AllowNonStringObjectKeys) && pair.first.type() != variant::Str) {
        throw not_encodable("Cannot encode non-string map key type into JSON");
      }
      write(oss, pair.first);
      oss << ':';
      write(oss, pair.second);
    }
    oss << '}';
  } break;
  }
}

template<int N>
struct pow10L {
    enum : int64_t { value = 10 * pow10L<N - 1>::value };
};
template <>
struct pow10L<0> {
    enum : int64_t { value = 1 };
};
constexpr int64_t qpow10(int n) {
  constexpr int64_t lookupTable[] = {
    pow10L<0>::value,  pow10L<1>::value,  pow10L<2>::value,  pow10L<3>::value,
    pow10L<4>::value,  pow10L<5>::value,  pow10L<6>::value,  pow10L<7>::value,
    pow10L<8>::value,  pow10L<9>::value,  pow10L<10>::value, pow10L<11>::value,
    pow10L<12>::value, pow10L<13>::value, pow10L<14>::value, pow10L<15>::value,
    pow10L<16>::value, pow10L<17>::value, pow10L<18>::value
  };
  return lookupTable[n];
}

void encodeUtf8(std::string &dest, int codepoint) {
  char buf[6];
  if (codepoint <= 0x7F) {
    dest.append(1, static_cast<char>(codepoint));
  } else if (codepoint <= 0x7FF) {
    buf[0] = 0b11000000 | (codepoint >> 6);
    buf[1] = 0b10000000 | (codepoint & 0b00111111);
    dest.append(buf, 2);
  } else if (codepoint <= 0xFFFF) {
    buf[0] = 0b11100000 | (codepoint >> 12);
    buf[1] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
    buf[2] = 0b10000000 | (codepoint & 0b00111111);
    dest.append(buf, 3);
  } else if (codepoint <= 0x1FFFFF) {
    buf[0] = 0b11110000 | (codepoint >> 18);
    buf[1] = 0b10000000 | ((codepoint >> 12) & 0b00111111);
    buf[2] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
    buf[3] = 0b10000000 | (codepoint & 0b00111111);
    dest.append(buf, 4);
  } else if (codepoint <= 0x3FFFFFF) {
    buf[0] = 0b11110000 | (codepoint >> 24);
    buf[1] = 0b10000000 | ((codepoint >> 18) & 0b00111111);
    buf[2] = 0b10000000 | ((codepoint >> 12) & 0b00111111);
    buf[3] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
    buf[4] = 0b10000000 | (codepoint & 0b00111111);
    dest.append(buf, 5);
  } else if (codepoint <= 0x7FFFFFFF) {
    buf[0] = 0b11110000 | (codepoint >> 30);
    buf[1] = 0b10000000 | ((codepoint >> 24) & 0b00111111);
    buf[2] = 0b10000000 | ((codepoint >> 18) & 0b00111111);
    buf[3] = 0b10000000 | ((codepoint >> 12) & 0b00111111);
    buf[4] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
    buf[5] = 0b10000000 | (codepoint & 0b00111111);
    dest.append(buf, 6);
  } else {
    throw std::invalid_argument("Invalid codepoint");
  }
}

void read(std::istringstream &iss, variant &v, ReadFlags rf) {
  int c;
  constexpr auto eof = std::istringstream::traits_type::eof();
  struct ISSException {
    // RAII-style "finally" handler to restore the input stream's exception settings
    std::istringstream &iss;
    std::ios_base::iostate exc;
    ISSException(std::istringstream &iss) : iss(iss) {
      exc = iss.exceptions();
    }
    ~ISSException() {
      iss.exceptions(exc);
    }
  } issException(iss);
  using ios = std::ios_base;
  iss.exceptions(ios::badbit);
  auto skipWhitespace = [&iss, &c]() {
    do {
      c = iss.get();
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
  };
  skipWhitespace();
  if (c == '{') {
    v.type(variant::Map);
    skipWhitespace();
    if (iss.eof()) {
      throw unexpected_eof("Unexpected EOF while parsing object, expected key or object end");
    }
    if (c != '}') while (true) {
      variant key;
      iss.unget();
      read(iss, key);
      if (rf & EnforceStringObjectKeys && !key.isStr()) {
        throw invalid_input("Non-string as JSON object key");
      }
      skipWhitespace();
      if (iss.eof()) {
        throw unexpected_eof("Unexpected EOF while parsing object entry, expected \":\"");
      }
      if (c != ':') {
        throw invalid_input("Expected key-value separator \":\"");
      }
      skipWhitespace();
      if (iss.eof()) {
        throw unexpected_eof("Unexpected EOF while parsing object entry, expected value");
      }
      iss.unget();
      auto it = v.getMap().emplace(std::piecewise_construct,
          std::forward_as_tuple(key), std::forward_as_tuple());
      read(iss, it.first->second);
      skipWhitespace();
      if (iss.eof()) {
        throw unexpected_eof("Unexpected EOF while parsing object, expected object end or comma");
      }
      if (c == '}') {
        break;
      } else if (c != ',') {
        throw invalid_input("Unexpected character inbetween object elements");
      }
      skipWhitespace();
      if (iss.eof()) {
        throw unexpected_eof("Unexpected EOF while parsing object, expected key");
      }
    }
  } else if (c == '[') {
    v.type(variant::Array);
    skipWhitespace();
    if (iss.eof()) {
      throw unexpected_eof("Unexpected EOF while parsing array, expected value or array end");
    }
    if (c != ']') while (true) {
      iss.unget();
      v.getArray().emplace_back();
      read(iss, v.getArray().back());
      skipWhitespace();
      if (iss.eof()) {
        throw unexpected_eof("Unexpected EOF while parsing array, expected array end or comma");
      }
      if (c == ']') {
        break;
      } else if (c != ',') {
        throw invalid_input("Unexpected character inbetween array elements");
      }
      skipWhitespace();
      if (iss.eof()) {
        throw unexpected_eof("Unexpected EOF while parsing array, expected value");
      }
    }
  } else if (c == '"') {
    v.type(variant::Str);
    std::string &str = v.getStr();
    while (true) {
      c = iss.get();
      if (c == '\\') {
        c = iss.get();
        switch (c) {
        case '"':
          str.append(1, '"');
          break;
        case '\\':
          str.append(1, '\\');
          break;
        case '/':
          str.append(1, '/');
          break;
        case 'b':
          str.append(1, '\x08');
          break;
        case 'f':
          str.append(1, '\x0C');
          break;
        case 'n':
          str.append(1, '\n');
          break;
        case 'r':
          str.append(1, '\r');
          break;
        case 't':
          str.append(1, '\t');
          break;
        case 'u': {
          std::string hex(4, '\0');
          iss.read(&hex[0], 4);
          if (iss.eof()) {
            throw unexpected_eof("Unexpected EOF while parsing unicode escape");
          }
          int unichar = std::stoi(hex, nullptr, 16);
          if (unichar >= 0xD800 && unichar <= 0xDBFF) {
            // If the character is a high surrogate, read the low surrogate
            int backslash = iss.get();
            int u = iss.peek();
            if (backslash != '\\' || u != 'u') {
              iss.unget();
              if (rf & ReplaceInvalidCodepoints) {
                str.append("�");
              } else if (!(rf & IgnoreInvalidCodepoints)) {
                throw invalid_input("Missing low surrogate while parsing surrogate pair");
              }
              break;
            }
            iss.get();
            iss.read(&hex[0], 4);
            if (iss.eof()) {
              throw unexpected_eof("Unexpected EOF while parsing unicode low surrogate");
            }
            int lowsurrogate = std::stoi(hex, nullptr, 16);
            if (!(lowsurrogate >= 0xDC00 && lowsurrogate <= 0xDFFF)) {
              if (rf & ReplaceInvalidCodepoints) {
                str.append("�");
              } else if (!(rf & IgnoreInvalidCodepoints)) {
                throw invalid_input("Invalid low surrogate while parsing surrogate pair");
              }
              break;
            }
            encodeUtf8(str, 0x10000 | ((unichar & 0b1111111111) << 10) | (lowsurrogate & 0b1111111111));
          } else if (unichar >= 0xDC00 && unichar <= 0xDFFF) {
            // If the character is a low surrogate, it's missing its higher pair
            if (rf & ReplaceInvalidCodepoints) {
              str.append("�");
            } else if (!(rf & IgnoreInvalidCodepoints)) {
              throw unexpected_input("Unexpected low surrogate while parsing string");
            }
          } else {
            encodeUtf8(str, unichar);
          }
        } break;
        case 'x': {
          std::string hex(2, '\0');
          iss.read(&hex[0], 2);
          str.append(1, static_cast<char>(std::stoi(hex, nullptr, 16)));
        } break;
        case eof:
          throw unexpected_eof("Unexpected EOF while parsing string escape sequence");
        }
      } else if (c == '"') {
        break;
      } else if (c == eof) {
        throw unexpected_eof("Unexpected EOF while parsing string");
      } else {
        str.append(1, static_cast<char>(c));
      }
    }
  } else if (c == '-' || (c >= '0' && c <= '9')) {
    std::string num(1, static_cast<char>(c)), exp;
    bool decimal = false, exponent = false;
    auto readDigits = [&iss, &num, &exp, &c, &decimal, &exponent]() {
      while (iss.good()) {
        c = iss.peek();
        if (c < '0' || c > '9')
          break;
        iss.ignore();
        ((!decimal && exponent) ? exp : num) += static_cast<char>(c);
      }
    };
    readDigits();
    if (c == '.') {
      decimal = true;
      num += '.';
      iss.ignore();
      readDigits();
    }
    if (c == 'e' || c == 'E') {
      exponent = true;
      if (decimal) {
        num += 'e';
      }
      iss.ignore();
      c = iss.peek();
      if (c == '+' || c == '-') {
        ((!decimal && exponent) ? exp : num) += static_cast<char>(iss.get());
      }
      readDigits();
    }
    if (decimal) {
      v = std::atof(num.c_str());
    } else {
      using std::atoll;
      if (exponent) {
        auto expn = atoll(exp.c_str());
        if (expn < 1 || expn >= 19) {
          num += "e";
          num += exp;
          v = std::atof(num.c_str());
        } else {
          v = static_cast<int64_t>(atoll(num.c_str())) * static_cast<int64_t>(qpow10(expn));
        }
      } else {
        v = static_cast<int64_t>(atoll(num.c_str()));
      }
    }
  } else if (c == 'n') {
    char ull[3] = {};
    iss.read(ull, 3);
    if (iss.eof()) {
      throw unexpected_eof("Unexpected EOF, expected \"null\"");
    }
    if (memcmp(ull, "ull", 3) != 0) {
      throw unexpected_input("Unexpected character; expected \"null\" expression");
    }
    v = nullptr;
  } else if (c == 't') {
    char rue[3] = {};
    iss.read(rue, 3);
    if (iss.eof()) {
      throw unexpected_eof("Unexpected EOF, expected \"true\"");
    }
    if (memcmp(rue, "rue", 3) != 0) {
      throw unexpected_input("Unexpected character; expected \"true\" expression");
    }
    v = true;
  } else if (c == 'f') {
    char alse[4] = {};
    iss.read(alse, 4);
    if (iss.eof()) {
      throw unexpected_eof("Unexpected EOF, expected \"false\"");
    }
    if (memcmp(alse, "alse", 4) != 0) {
      throw unexpected_input("Unexpected character; expected \"false\" expression");
    }
    v = false;
  } else if (c == eof) {
    throw unexpected_eof("Unexpected EOF");
  } else {
    throw unexpected_input("Unexpected character");
  }
}

}
}
