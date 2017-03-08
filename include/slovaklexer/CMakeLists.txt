cmake_minimum_required(VERSION 2.6)

# May be version conflict
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/btree.h DESTINATION include/slovaklexer)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TCPFork.h DESTINATION include/slovaklexer)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/RagelProcessor.h DESTINATION include/slovaklexer)

add_custom_command (
    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/../SlovakLexer.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h.rl # build when lexer.rl is newer
    COMMAND ragel -o ${CMAKE_CURRENT_SOURCE_DIR}/../SlovakLexer.h ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h.rl
)
add_custom_target(lexer2 DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../SlovakLexer.h)


add_definitions(-DNETWORKING_ENABLED)
add_custom_command (
    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h.rl # build when lexer.rl is newer
    COMMAND ragel -o ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h.rl
)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h DESTINATION include/slovaklexer)
add_custom_target(annotator DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h)


add_custom_command (
  OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h.rl # build when lexer.rl is newer
  COMMAND ragel -o ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h.rl
)
add_custom_target(uliceh DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h DESTINATION include/slovaklexer)


 
