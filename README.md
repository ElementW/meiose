# meiose, a MessagePack (and JSON) de/serialization library

The core component of the library is the `meiose::variant`, a tagged union
container supporting the types of the MessagePack specification.

Use the `read` and `write` functions of the `msgpack` and `json` sub-namespaces
to read and write from and to `std::{i,o}stringstream`, respectively.

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
