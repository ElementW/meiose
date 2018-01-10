#include <assert.h>
#include <meiose/variant.hpp>
#include <meiose/json.hpp>

#include "TestUtils.hpp"

using namespace meiose;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  assert(json::write({}) == "null");
  assert(json::write(true) == "true");
  assert(json::write(false) == "false");
  assert_exception(json::write(binary()), std::runtime_error&);
  assert_exception(json::write(extension()), std::runtime_error&);
  assert(json::write(12.34) == "12.34");  // Assumes good enough precision
  assert(json::write(98765) == "98765");
  assert(json::write("\1I'm a \\ test with a \" quote\n") == "\"\\u0001I'm a \\\\ test with a \\\" quote\\n\"");
  assert(json::write("\x08\x09\x0A\x0B\x0C\x0D") == "\"\\b\\t\\n\\u000B\\f\\r\"");
  assert(json::write(array { 1, 7.7, "test", array(), map() }) == "[1,7.7,\"test\",[],{}]");
  assert(json::write(map { {"one", array()}, {"two", -42} }) == "{\"one\":[],\"two\":-42}");
  assert_exception(json::write(map { {array(), 0} }), std::runtime_error&);

  return 0;
}
