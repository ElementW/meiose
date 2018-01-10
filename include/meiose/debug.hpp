#ifndef MEIOSE_DEBUG_HPP
#define MEIOSE_DEBUG_HPP

#include <sstream>

#include "variant.hpp"

namespace meiose {
namespace debug {

void dump(std::ostringstream &oss, const variant &v, const std::string &prefix,
          bool skipindent = false);

inline std::string dump(const variant &v) {
  std::ostringstream oss;
  dump(oss, v, "");
  return oss.str();
}

}
}

#endif /* MEIOSE_DEBUG_HPP */
