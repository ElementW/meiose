#ifndef MEIOSE_JSON_HPP
#define MEIOSE_JSON_HPP

#include <sstream>

#include "exceptions.hpp"
#include "variant.hpp"

namespace meiose {
namespace json {

using WriteFlags = int;
constexpr WriteFlags ByteXEscape = 1;
constexpr WriteFlags AllowNonStringObjectKeys = 2;

void write(std::ostringstream &oss, const variant &v, WriteFlags = 0);

inline std::string write(const variant &v, WriteFlags wf = 0) {
  std::ostringstream oss;
  write(oss, v, wf);
  return oss.str();
}


using ReadFlags = int;
constexpr ReadFlags ReplaceInvalidCodepoints = 1;
constexpr ReadFlags IgnoreInvalidCodepoints = 2;
constexpr ReadFlags EnforceStringObjectKeys = 4;

void read(std::istringstream &iss, variant &v, ReadFlags = 0);

inline variant read(std::istringstream &iss, ReadFlags rf = 0) {
  variant v;
  read(iss, v, rf);
  return v;
}

inline variant read(const std::string &str, ReadFlags rf = 0) {
  variant v;
  std::istringstream iss(str);
  read(iss, v, rf);
  return v;
}

}
}

#endif /* MEIOSE_JSON_HPP */
