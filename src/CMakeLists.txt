cmake_minimum_required(VERSION 2.6)
add_definitions(-DNETWORKING_ENABLED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lemonstring/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_executable(slovaktokenizer tokenizer2.cpp)
target_link_libraries(slovaktokenizer lemonstring)
add_dependencies(slovaktokenizer lexer2)
install(TARGETS slovaktokenizer DESTINATION bin)
add_executable(ngram-replace ngram-replace.cpp )
add_dependencies(ngram-replace annotator)
target_link_libraries(ngram-replace lemonstring)
install(TARGETS ngram-replace DESTINATION bin)

add_executable(cisloslovom cisloslovom.cpp)
target_link_libraries(cisloslovom lemonstring)
install(TARGETS cisloslovom DESTINATION bin)

add_executable(ulice ulice.cpp)
target_link_libraries(ulice lemonstring)
add_dependencies(ulice uliceh)


add_executable(dedup dedup.cpp)
install(TARGETS dedup DESTINATION bin)
