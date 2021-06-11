cmake_minimum_required(VERSION 3.17)
project(graph_theory)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(graph_theory src/main.cpp src/graph.hpp src/utility/maybe.hpp src/utility/maybe.hpp src/representation/list_graph.hpp)
target_link_libraries(graph_theory ${CONAN_LIBS})