cmake_minimum_required(VERSION 2.6)

# May be version conflict
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/btree.h DESTINATION include/slovaklexer)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TCPFork.h DESTINATION include/slovaklexer)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/RagelProcessor.h DESTINATION include/slovaklexer)


add_definitions(-DNETWORKING_ENABLED)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/AnnotatorLexer.h DESTINATION include/slovaklexer)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Ulice.h DESTINATION include/slovaklexer)


 
