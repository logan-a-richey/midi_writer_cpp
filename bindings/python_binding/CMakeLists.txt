cmake_minimum_required(VERSION 3.12)
project(midiwriter_python)

set(CMAKE_CXX_STANDARD 17)

# pybind11
find_package(pybind11 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/../../include
)

add_library(midiwriter SHARED
    ../../src/midi_writer.cpp
    ../../src/track.cpp
    ../../bindings/python_binding/midi_pybind.cpp
)

target_link_libraries(midiwriter PRIVATE pybind11::module)
set_target_properties(midiwriter PROPERTIES PREFIX "" SUFFIX ".so")
