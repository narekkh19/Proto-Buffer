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