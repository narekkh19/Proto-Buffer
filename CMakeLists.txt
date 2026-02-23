cmake_minimum_required(VERSION 3.20)
project(ProtoBufferProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# No spaces in paths = No headaches
include_directories(ProtocCompiler)

add_executable(ProtocCompiler
    "ProtocCompiler/CompilerMain.cpp"
 "ProtocCompiler/Parser.cpp")