cmake_minimum_required(VERSION 2.6)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CisloSlovom.h DESTINATION include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/SlovakCisloSlovom.h DESTINATION include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/SlovakLexer.h DESTINATION include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/SlovakReplace.h DESTINATION include)

add_subdirectory(slovaklexer)
