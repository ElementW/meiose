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
