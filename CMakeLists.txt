cmake_minimum_required(VERSION 3.8)
project(opt-rule-eval-cs236-byu)

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror=sign-compare -g -O3")

file(GLOB SOURCES "./src/*")
add_executable(opt-rule-eval-cs236-byu ${SOURCES})