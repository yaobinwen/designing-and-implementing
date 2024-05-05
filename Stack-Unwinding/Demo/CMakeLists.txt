cmake_minimum_required(VERSION 3.10)

# Specify the C standard
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)

# ##################################################

# Set the project name.
project(
    demo_stack_unwinding_01
    DESCRIPTION
    "Demo termination during stack unwinding when exception's copy construction throws"
)

# Add the executable.
add_executable(
    demo_stack_unwinding_01
    "./stack-unwinding-01.cpp"
)

# Add the include and library directories.
target_include_directories(demo_stack_unwinding_01 SYSTEM PUBLIC)
target_link_libraries(demo_stack_unwinding_01)

# ##################################################

# Set the project name.
project(
    demo_stack_unwinding_02
    DESCRIPTION
    "Demo termination during stack unwinding when an automatic object's destructor throws"
)

# Add the executable.
add_executable(
    demo_stack_unwinding_02
    "./stack-unwinding-02.cpp"
)

# Add the include and library directories.
target_include_directories(demo_stack_unwinding_02 SYSTEM PUBLIC)
target_link_libraries(demo_stack_unwinding_02)
