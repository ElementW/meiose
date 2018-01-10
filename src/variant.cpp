#include <meiose/meiose.hpp>

namespace meiose {

const variant variant::const_nil(nullptr);
const variant::binary variant::const_bin;
const variant::extension variant::const_ext;
const variant::string variant::const_str;
const variant::array variant::const_arr;
const variant::map variant::const_map;

void variant::construct() {
  switch (m_type) {
  case Bin:
    new (&v_bin) binary;
    break;
  case Ext:
    new (&v_ext) extension;
    break;
  case Str:
    new (&v_str) string;
    break;
  case Array:
    new (&v_arr) array;
    break;
  case Map:
    new (&v_map) map;
    break;
  default:
    v_i = 0;
    break;
  }
}

void variant::clear() {
  switch (m_type) {
  case Bin:
    v_bin.clear();
    break;
  case Ext:
    v_ext.first = decltype(v_ext.first)();
    v_ext.second.clear();
    break;
  case Str:
    v_str.clear();
    break;
  case Array:
    v_arr.clear();
    break;
  case Map:
    v_map.clear();
    break;
  default:
    v_i = 0;
    break;
  }
}

void variant::destroy() {
  switch (m_type) {
  case Bin:
    v_bin.~vector();
    break;
  case Ext:
    v_ext.~pair();
    break;
  case Str:
    v_str.~string();
    break;
  case Array:
    v_arr.~vector();
    break;
  case Map:
    v_map.~map();
    break;
  default:
    break;
  }
}

void variant::type(Type t) {
  if (t == m_type) {
    clear();
    return;
  }
  destroy();
  m_type = t;
  construct();
}

}
