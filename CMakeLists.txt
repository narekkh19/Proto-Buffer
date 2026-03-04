cmake_minimum_required(VERSION 3.20)
project(ProtoBufferProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(CompilerLogic STATIC
    "ProtocCompiler/src/Parser.cpp"
    "ProtocCompiler/src/Generator.cpp"
)

add_executable(MyCompiler "ProtocCompiler/src/CompilerMain.cpp")

target_link_libraries(MyCompiler PRIVATE CompilerLogic)

target_include_directories(MyCompiler PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ProtocCompiler/src")

# --- GoogleTest Setup ---
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- ProtoTests Executable ---
add_executable(ProtoTests
    "tests/test_person.cpp"
    "ProtocCompiler/generated/Person.prt.cpp"
)

target_include_directories(ProtoTests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/ProtocCompiler/generated"
    "${CMAKE_CURRENT_SOURCE_DIR}/ProtocCompiler/include"
)

target_link_libraries(ProtoTests PRIVATE gtest_main)

add_test(NAME ProtoTests COMMAND ProtoTests)