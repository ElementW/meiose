project('meiose', 'cpp')
src = ['src/debug.cpp', 'src/json.cpp', 'src/msgpack.cpp', 'src/variant.cpp']
inc = include_directories('include')
lib = library('meiose', src, include_directories: inc)
meiose = declare_dependency(include_directories: inc, link_with: lib)
