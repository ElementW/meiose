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
#ifndef MEIOSE_JSON_HPP
#define MEIOSE_JSON_HPP

#include <istream>
#include <ostream>
#include <sstream>

#include "exceptions.hpp"
#include "variant.hpp"

namespace meiose {
namespace json {

using WriteFlags = int;
constexpr WriteFlags ByteXEscape = 1;
constexpr WriteFlags AllowNonStringObjectKeys = 2;

void write(std::ostream &oss, const variant &v, WriteFlags = 0);

inline std::string write(const variant &v, WriteFlags wf = 0) {
  std::ostringstream oss;
  write(oss, v, wf);
  return oss.str();
}


using ReadFlags = int;
constexpr ReadFlags ReplaceInvalidCodepoints = 1;
constexpr ReadFlags IgnoreInvalidCodepoints = 2;
constexpr ReadFlags EnforceStringObjectKeys = 4;

void read(std::istream &iss, variant &v, ReadFlags = 0);

inline variant read(std::istream &iss, ReadFlags rf = 0) {
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
