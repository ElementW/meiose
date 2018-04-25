TEMPLATE = lib
CONFIG += c++14
CONFIG -= qt

!meiose-shared {
  CONFIG += staticlib
}
!meiose-shared | meiose-debug {
  QMAKE_CXXFLAGS += -g
}

INCLUDEPATH += include

SOURCES += \
  src/debug.cpp \
  src/json.cpp \
  src/msgpack.cpp \
  src/variant.cpp

HEADERS += \
  include/meiose/debug.hpp \
  include/meiose/endian.hpp \
  include/meiose/exceptions.hpp \
  include/meiose/json.hpp \
  include/meiose/meiose.hpp \
  include/meiose/msgpack.hpp \
  include/meiose/variant.hpp
