cmake_minimum_required(VERSION 2.6)
add_definitions(-DNETWORKING_ENABLED)

# May be version conflict
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/btree.h DESTINATION include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TCPFork.h DESTINATION include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/RagelProcessor.h DESTINATION include)

add_custom_command (
    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h.rl # build when lexer.rl is newer
    COMMAND ragel -o ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h.rl
)
add_custom_target(lexer2 DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h DESTINATION include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(slovaktokenizer tokenizer2.cpp)
target_link_libraries(slovaktokenizer lemonstring)
add_dependencies(slovaktokenizer lexer2)
install(TARGETS slovaktokenizer DESTINATION bin)
add_definitions(-DNETWORKING_ENABLED)
add_custom_command (
    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h.rl # build when lexer.rl is newer
    COMMAND ragel -o ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h.rl
)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h DESTINATION include)


add_custom_target(annotator DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h)
add_executable(ngram-replace ngram-replace.cpp )
add_dependencies(ngram-replace annotator)
target_link_libraries(ngram-replace lemonstring)
install(TARGETS ngram-replace DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/SlovakReplace.h DESTINATION include)

add_executable(cisloslovom cisloslovom.cpp)
target_link_libraries(cisloslovom lemonstring)
install(TARGETS cisloslovom DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CisloSlovom.h DESTINATION include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/SlovakCisloSlovom.h DESTINATION include)

add_custom_command (
  OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h.rl # build when lexer.rl is newer
  COMMAND ragel -o ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h.rl
)
add_custom_target(uliceh DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h DESTINATION include)

add_executable(ulice ulice.cpp)
target_link_libraries(ulice lemonstring)
add_dependencies(ulice uliceh)


 
