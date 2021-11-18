cmake_minimum_required(VERSION 2.6)
include_directories(../include)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(SOURCES
Tokenizer.cpp
floatparsef.cpp
utf8proc.c
Command.cpp
UTokenizer.cpp
)

add_library(lemonstring ${SOURCES})
install(TARGETS lemonstring DESTINATION lib)

