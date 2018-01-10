#include <meiose/debug.hpp>

#include <iomanip>

namespace meiose {
namespace debug {

void dump(std::ostringstream &oss, const variant &v, const std::string &prefix,
          bool skipindent) {
  if (not skipindent) {
    oss << prefix;
  }
  switch (v.type()) {
  case variant::Nil:
    oss << "nil";
    break;
  case variant::Bool:
    oss << (v.operator bool() ? "true" : "false");
    break;
  case variant::Bin: {
    auto &bin = v.operator const variant::binary&();
    oss << "<binary length:" << bin.size() << " data:" << std::hex << std::setfill('0') <<
           std::uppercase;
    for (size_t i = 0; i < bin.size(); ++i) {
      oss << std::setw(2) << static_cast<int>(bin[i]) << ' ';
    }
    oss << std::dec << std::nouppercase << ">";
  } break;
  case variant::Ext: {
    auto &ext = v.operator const variant::extension&();
    oss << "<extension type:" << static_cast<int>(ext.first) << " length:" <<
           ext.second.size() << " data:" << std::hex << std::setfill('0') << std::uppercase;
    for (size_t i = 0; i < ext.second.size(); ++i) {
      oss << std::setw(2) << static_cast<int>(ext.second[i]) << ' ';
    }
    oss << std::dec << std::nouppercase << ">";
  } break;
  case variant::Float:
    oss << v.operator double();
    break;
  case variant::Int:
    oss << v.operator int64_t();
    break;
  case variant::Str:
    oss << '"' << v.operator const std::string&() << '"';
    break;
  case variant::Array:
    oss << "[\n";
    for (auto &elm : v.operator const variant::array&()) {
      dump(oss, elm, prefix + "  ");
      oss << ",\n";
    }
    oss << prefix << "]";
    break;
  case variant::Map:
    oss << "{\n";
    for (auto &pair : v.operator const variant::map&()) {
      dump(oss, pair.first, prefix + "  ");
      oss << ": ";
      dump(oss, pair.second, prefix + "  ", true);
      oss << ",\n";
    }
    oss << prefix << "}";
    break;
  }
}

}
}
