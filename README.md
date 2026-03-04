# Proto-Buffer C++ Implementation

A **custom Protocol Buffer (protobuf) implementation in C++20** designed for **learning and educational purposes**. This project demonstrates how to build a serialization/deserialization system similar to Google's Protocol Buffers from scratch.

## 📌 Project Overview

This is a lightweight, educational protobuffer system written in modern C++20. It provides:

- **Custom `.proto` schema parsing** — Parse simple protocol buffer schema definitions
- **Code generation** — Auto-generate C++ classes from `.proto` schemas
- **Serialization/Deserialization** — Encode and decode messages to/from binary format
- **Type support** — Strings, integers, enums, nested messages, and vectors
- **Unit testing** — Comprehensive tests using Google Test (GTest)

## 🎓 Learning Purpose

This project is meant to:
- Understand how protocol buffers work under the hood
- Learn about serialization and deserialization
- Explore Varint and ZigZag encoding
- Practice C++ template metaprogramming
- Understand code generation patterns

**Note:** This is an educational implementation and is **not intended for production use**.

## 🏗️ Project Structure

```
ProtoBuffer/
├── ProtocCompiler/
│   ├── src/                    # Compiler source code
│   │   ├── Parser.cpp
│   │   ├── Generator.cpp
│   │   └── CompilerMain.cpp
│   ├── include/                # Header files
│   │   ├── Parser.hpp
│   │   ├── Generator.hpp
│   │   ├── AST.hpp
│   │   └── ProtoRuntime.hpp
│   ├── generated/              # Generated code from schema
│   │   ├── Person.prt.hpp
│   │   ├── Person.prt.cpp
│   │   └── ProtoRuntime.hpp
│   └── tests/                  # Parser tests
│       └── test_parser.cpp
├── tests/                      # Schema tests
│   └── test_person.cpp
├── CMakeLists.txt
└── README.md
```

## 🚀 Quick Start

### Prerequisites

- **CMake** >= 3.20
- **C++20 compatible compiler** (MSVC, GCC, or Clang)
- **Ninja** build generator (or CMake's default)
- **Git** (for cloning)

### Building the Project

```bash
# Clone the repository
git clone https://github.com/narekkh19/Proto-Buffer.git
cd Proto-Buffer

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -G "Ninja" ..

# Build the project
cmake --build .
```

### Running Tests

```bash
# Run all tests
ctest --output-on-failure

# Or from the build directory
./ProtoTests         # On Linux/macOS
.\ProtoTests.exe     # On Windows
```

### Using the Compiler

```bash
# Compile a .proto schema
./MyCompiler /path/to/schema.proto

# This generates:
# - schema.prt.hpp  (header file with class definitions)
# - schema.prt.cpp  (implementation with serialize/deserialize)
```

## 📖 Example Usage

### Define a Schema

Create `person.proto`:
```protobuf
message Person {
  string name = 1;
  int32 id = 2;
  string email = 3;
  
  message PhoneNumber {
    string number = 1;
    PhoneType type = 2;
  }
  
  repeated PhoneNumber phones = 4;
}

message AddressBook {
  repeated Person people = 1;
}
```

### Use Generated Code

```cpp
#include "Person.prt.hpp"

int main() {
    using namespace tutorial;
    
    // Create a person
    Person person;
    person.set_name("Alice");
    person.set_id(123);
    person.set_email("alice@example.com");
    
    // Add a phone number
    Person::PhoneNumber phone;
    phone.set_number("555-1234");
    phone.set_type(Person::PhoneType::PHONE_TYPE_MOBILE);
    person.add_phones(phone);
    
    // Serialize
    auto buffer = person.Serialize();
    
    // Deserialize
    Person person2;
    person2.Deserialize(buffer);
    
    std::cout << person2.get_name() << std::endl;  // Output: Alice
    
    return 0;
}
```

## 🧪 Testing

The project includes comprehensive unit tests using **Google Test**:

- **`test_person.cpp`** — Tests serialization/deserialization of the Person schema
- **`test_parser.cpp`** — Tests the schema parser

Tests verify:
- ✅ Round-trip serialization (serialize → deserialize → verify equality)
- ✅ Multiple nested objects
- ✅ Repeated fields (vectors)
- ✅ Enum handling
- ✅ String fields and modifications

Run tests in Visual Studio:
1. Open **Test → Test Explorer**
2. Click **Run All**
3. See green checkmarks for passing tests ✓

## 🔧 Technical Details

### Encoding

- **Varint** — Efficient encoding of variable-length integers
- **ZigZag** — Compact encoding of signed integers
- **Wire Types** — 0 (varint), 1 (fixed64), 2 (length-delimited), 5 (fixed32)

### Data Types Supported

| Type | Example |
|------|---------|
| `int32` / `int64` | `int32 age = 1;` |
| `float` / `double` | `float score = 1;` |
| `string` | `string name = 1;` |
| `enum` | `PhoneType type = 1;` |
| `message` | `PhoneNumber phone = 1;` |
| `repeated` | `repeated Phone phones = 1;` |

## 📚 Key Features

- ✅ Custom schema parser (simplified protobuf syntax)
- ✅ Automatic C++ code generation
- ✅ Serialization to binary format
- ✅ Deserialization from binary format
- ✅ Nested messages support
- ✅ Enum support
- ✅ Repeated fields (vectors)
- ✅ Full unit test coverage
- ✅ Modern C++20 implementation

## ⚠️ Limitations

This is an educational project with intentional limitations:

- Simplified `.proto` syntax (not fully compatible with protoc)
- No optional/required field distinctions
- No field deprecation support
- No extensions
- No RPC support
- No protobuf3 syntax support

## 🤝 Contributing

While this is primarily a learning project, feel free to:
- Report issues
- Suggest improvements
- Add more test cases
- Improve documentation

## 📝 License

This project is provided as-is for educational purposes.

## 🎯 Future Learning Enhancements

Potential improvements for learning:

- [ ] Full protobuf3 syntax support
- [ ] Proto2 backwards compatibility
- [ ] Custom serialization strategies
- [ ] Performance benchmarking
- [ ] More complex nested structures
- [ ] Reflection/descriptor API
- [ ] gRPC integration (advanced learning)

## 📞 Contact & Questions

For questions about this educational project, feel free to open an issue on GitHub.

---

**Remember:** This project is meant to demystify how Protocol Buffers work. For production use, please use the official [Google Protocol Buffers](https://github.com/protocolbuffers/protobuf) library.

**Happy Learning! 🚀**
