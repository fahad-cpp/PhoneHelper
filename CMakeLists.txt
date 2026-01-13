set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

cmake_minimum_required(VERSION 3.10)
project(PhoneHelper)
file(GLOB SOURCES *.cpp)
add_executable(PhoneHelper ${SOURCES})