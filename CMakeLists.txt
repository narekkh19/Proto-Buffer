cmake_minimum_required(VERSION 3.20)
project(ProtoBufferProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. DOWNLOAD GOOGLE TEST ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# This prevents GTest from creating conflicts with your compiler settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- 2. YOUR PROJECT SOURCE ---
# Include your source folder
include_directories(ProtocCompiler)

# Define which files make up your logic (everything EXCEPT main)
# We do this so the tests can use your Parser/AST without running main()
add_library(CompilerLib STATIC 
    "ProtocCompiler/Parser.cpp"
)

# Your original executable
add_executable(ProtocCompiler "ProtocCompiler/CompilerMain.cpp")
target_link_libraries(ProtocCompiler PRIVATE CompilerLib)

# --- 3. THE TEST SUITE ---
# Create the test executable
add_executable(ParserTests "ProtocCompiler/tests/test_parser.cpp")

# Link to GTest and your Compiler logic
# 'GTest::gtest_main' provides the main() function for the tests automatically
target_link_libraries(ParserTests PRIVATE CompilerLib GTest::gtest_main)

# Tell Visual Studio to look for tests in this executable
include(GoogleTest)
gtest_discover_tests(ParserTests)