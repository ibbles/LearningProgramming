cmake_minimum_required(VERSION 3.20)
project(taskflow-notes VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)


# This may require
#  $ sudo sysctl vm.mmap_rnd_bits=30
# to prevent
#  FATAL: ThreadSanitizer: unexpected memory mapping
# See
# - https://stackoverflow.com/questions/77850769/fatal-threadsanitizer-unexpected-memory-mapping-when-running-on-linux-kernels

set(CMAKE_CXX_FLAGS "-fsanitize=thread")
set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=thread")

include(FetchContent)


FetchContent_Declare(
    taskflow
    GIT_REPOSITORY https://github.com/taskflow/taskflow.git
    GIT_TAG 0007dd38512b704b6c7e456f0b9499139bbb36ef  # Some random commit.
    SOURCE_DIR "${CMAKE_BINARY_DIR}/taskflow"
)
FetchContent_GetProperties(taskflow)
if(NOT taskflow_POPULATED)
    FetchContent_Populate(taskflow)
    add_library(taskflow INTERFACE)
    target_include_directories(taskflow SYSTEM INTERFACE "${taskflow_SOURCE_DIR}")
endif()

function(add_example name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} taskflow)
endfunction()


add_example(a_simple_example)
add_example(creating_dependencies)
add_example(creating_multiple_tasks)
add_example(dynamic_task)
add_example(succeeds_parent_task)
add_example(composed_tasks)
add_example(branch_and_loop)
