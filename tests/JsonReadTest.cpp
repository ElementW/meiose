#include <cassert>
#include <meiose/variant.hpp>
#include <meiose/json.hpp>

#include "TestUtils.hpp"

using namespace meiose;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  assert(json::read("0").isInt());
  assert(json::read("0") == 0);

  assert(json::read(" 0").isInt());
  assert(json::read(" 0") == 0);

  assert(json::read("\n0").isInt());
  assert(json::read("\n0") == 0);

  assert(json::read("0 ").isInt());
  assert(json::read("0 ") == 0);

  assert(json::read("-199999").isInt());
  assert(json::read("-199999") == -199999);

  assert(json::read("1e5").isInt());
  assert(json::read("1e5") == 100000);

  assert_float(json::read("0.1"), 0.1);
  assert_float(json::read("1."), 1.);
  assert_float(json::read("1.1"), 1.1);
  assert_float(json::read("1.0e1"), 10.0);
  assert_float(json::read("1.0E2"), 100.0);
  assert_float(json::read("1.0e+3"), 1000.0);
  assert_float(json::read("1.0E-1"), 0.1);
  assert_float(json::read("1e-1"), 0.1);

  // Strings
  assert(json::read("\"\"").isStr());
  assert(json::read("\"\"") == "");

  assert(json::read("\"abc\"").isStr());
  assert(json::read("\"abc\"") == "abc");

  // Strings: basic escapes
  assert(json::read("\"abc\\u0001def\"") == "abc\x01""def");
  assert(json::read("\"abc\\u0005def\"") == "abc\x05""def");

  assert(json::read("\"abc\\bdef\"") == "abc\x08""def");
  assert(json::read("\"abc\\fdef\"") == "abc\x0C""def");
  assert(json::read("\"abc\\ndef\"") == "abc\ndef");
  assert(json::read("\"abc\\rdef\"") == "abc\rdef");
  assert(json::read("\"abc\\tdef\"") == "abc\tdef");

  assert(json::read("\"abc\\\\def\"") == "abc\\def");
  assert(json::read("\"abc\\/def\"") == "abc/def");
  assert(json::read("\"abc\\\"def\"") == "abc\"def");

  // Strings: \x escapes
  assert(json::read("\"abc\\x0Fdef\"") == "abc\x0F""def");
  assert(json::read("\"abc\\x1Adef\"") == "abc\x1A""def");

  // Strings: surrogate pair \u escapes
  assert(json::read("\"\\uD83D\\uDE00\"") == "😀");

  // Strings: unterminated escapes
  assert_exception(json::read("\"abc\\\""), meiose::read_error&);

  // Strings: EOF unterminated escapes
  assert_exception(json::read("\"abc\\"), meiose::unexpected_eof&);
  assert_exception(json::read("\"abc\\u"), meiose::unexpected_eof&);
  assert_exception(json::read("\"abc\\u1"), meiose::unexpected_eof&);
  assert_exception(json::read("\"abc\\u12"), meiose::unexpected_eof&);
  assert_exception(json::read("\"abc\\u123"), meiose::unexpected_eof&);
  assert_exception(json::read("\"abc\\uD83D"), meiose::read_error&);
  assert_exception(json::read("\"abc\\uD83D\\"), meiose::read_error&);
  assert_exception(json::read("\"abc\\uD83D\\u"), meiose::unexpected_eof&);
  assert_exception(json::read("\"abc\\uD83D\\uD"), meiose::unexpected_eof&);
  assert_exception(json::read("\"abc\\uD83D\\uDE"), meiose::unexpected_eof&);
  assert_exception(json::read("\"abc\\uD83D\\uDE0"), meiose::unexpected_eof&);

  // Arrays
  assert(json::read("[]").isArray());
  assert(json::read("[]") == variant(variant::Type::Array));
  assert(json::read("[1]").size() == 1);
  assert(json::read("[1,2]").size() == 2);
  assert(json::read("[1,2,3]").size() == 3);

  // Objects
  assert(json::read("{}").isMap());
  assert(json::read("{}") == variant(variant::Type::Map));
  { variant v = json::read("{\"a\":1}");
    assert(v.isMap());
    assert(v.size() == 1);
    assert(v["a"].isInt());
    assert(v["a"] == 1);
    assert_exception(v["b"], std::out_of_range&);
    assert_exception(v[0], std::out_of_range&);
  }
  { variant v = json::read("{\"z\":{}}");
    assert(v.isMap());
    assert(v.size() == 1);
    assert(v["z"].isMap());
    assert(v["z"].size() == 0);
    assert_exception(v["abcc"], std::out_of_range&);
    assert_exception(v[1254], std::out_of_range&);
  }
  { variant v = json::read("{1:[1,2,3]}"); // Non-compliant JSON, must still parse correctly
    assert(v.isMap());
    assert(v.size() == 1);
    assert(v[1].isArray());
    assert(v[1].size() == 3);
    assert_exception(v["nope"], std::out_of_range&);
    assert_exception(v[2], std::out_of_range&);
  }
  assert_exception(json::read("{1:[1,2,3]}", json::EnforceStringObjectKeys), invalid_input&);

  return 0;
}
