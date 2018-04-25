project('meiose', 'cpp', default_options: ['cpp_std=c++14'])
src = ['src/debug.cpp', 'src/json.cpp', 'src/msgpack.cpp', 'src/variant.cpp']
inc = include_directories('include')
if get_option('static')
  lib_static = static_library('meiose', src, include_directories: inc)
  meiose_static = declare_dependency(include_directories: inc, link_with: lib_static)
endif
if get_option('shared')
  lib_shared = shared_library('meiose', src, include_directories: inc)
  meiose = declare_dependency(include_directories: inc, link_with: lib_shared)
endif
