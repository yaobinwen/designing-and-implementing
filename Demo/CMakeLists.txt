cmake_minimum_required(VERSION 3.10)

# Specify the C standard
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)

# ##################################################

# Set the project name.
project(demo_vector DESCRIPTION "Write my simple vector")

# Add the executable.
add_executable(
    demo_vector
    "./vector/main.cpp"
)

# Add the include and library directories.
target_include_directories(demo_vector SYSTEM PUBLIC)
target_link_libraries(
    demo_vector
    gtest gtest_main pthread
)
