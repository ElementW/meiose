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
#ifndef MEIOSE_EXCEPTIONS_HPP
#define MEIOSE_EXCEPTIONS_HPP

#include <ios>
#include <stdexcept>

namespace meiose {

class write_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class not_encodable : public write_error {
public:
  using write_error::write_error;
};

class read_error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class invalid_input : public read_error {
public:
  using read_error::read_error;
};

class unexpected_input : public invalid_input {
public:
  using invalid_input::invalid_input;
};

class unexpected_eof : public std::ios_base::failure, public read_error {
public:
  unexpected_eof(const std::string &m) :
    failure(m), read_error(m) {
  }
  unexpected_eof(const char *m) :
    failure(m), read_error(m) {
  }
};

}

#endif /* MEIOSE_EXCEPTIONS_HPP */
