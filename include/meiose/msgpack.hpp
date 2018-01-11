#ifndef MEIOSE_MSGPACK_HPP
#define MEIOSE_MSGPACK_HPP

#include <sstream>

#include "variant.hpp"

namespace meiose {
namespace msgpack {

void write(std::ostream &oss, const variant &v);

inline std::string write(const variant &v) {
  std::ostringstream oss;
  write(oss, v);
  return oss.str();
}

void read(std::istream &iss, variant &v);

inline variant read(std::istream &iss) {
  variant v;
  read(iss, v);
  return v;
}

inline variant read(const std::string &str) {
  variant v;
  std::istringstream iss(str);
  read(iss, v);
  return v;
}

}
}

#endif /* MEIOSE_MSGPACK_HPP */
