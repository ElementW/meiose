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
#ifndef MEIOSE_MSGPACK_HPP
#define MEIOSE_MSGPACK_HPP

#include <istream>
#include <ostream>
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
