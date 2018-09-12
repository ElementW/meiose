# meiose, a C++14 MessagePack (and JSON) de/serialization library

The core component of the library is the `meiose::variant`, a tagged union
container supporting the types of the MessagePack specification.

Use the `read` and `write` functions of the `msgpack` and `json` sub-namespaces
to read and write from and to `std::{i,o}stream`, respectively.

meiose does *not* support streamed/partial reading or writing of values, it can
only read them or write them all at once.

## Build systems

Multiple build system files are available for easy integration of the library
among other codebases. By default, all build system compiles meiose
*statically*

* CMake: **this is the main supported build system**.
  Exports the following library names:
  * `meiose-static` if `MEIOSE_BUILD_STATIC` is true (default).
  * `meiose` is `MEIOSE_BUILD_SHARED` is true.
* Meson: defines the following dependency variables:
  * `meiose_static` if option `static` is true (default).
  * `meiose` if option `shared` is true.
* QMake: always builds a `meiose` library. Static by default,
  unless `meiose-shared` `CONFIG` is set.

## MsgPack type ↔ C++ type conversion

| MsgPack type | C++ type                                     |
| ------------ | -------------------------------------------- |
| Integer      | `int64_t`                                    |
| Nil          | `nullptr_t`                                  |
| Boolean      | `bool`                                       |
| Float        | `double`                                     |
| String       | `std::string`                                |
| Binary       | `std::vector<uint8_t>`                       |
| Array        | `std::vector<meiose::variant>`               |
| Map          | `std::map<meiose::variant, meiose::variant>` |
| Extension    | `std::pair<int8_t, std::vector<uint8_t>>`    |
