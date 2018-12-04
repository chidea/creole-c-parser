#
# CMake file for creole/c parser test frame.
#

include_directories( ${creole_SOURCE_DIR}/include )

link_directories( ${creole_BINARY_DIR}/lib )

set( parsertest_SRC main.cpp )
set( parsertest_HEADERS "${creole_SOURCE_DIR}/include/creole.h" )

source_group( "Header Files" FILES ${parsertest_HEADERS} )

add_executable( parser_test
    ${parsertest_SRC} ${parsertest_HEADERS} )

target_link_libraries( parser_test creole_lib ) 

set_target_properties( parser_test PROPERTIES
    COMPILE_DEFINITIONS CREOLE_LIB
    OUTPUT_NAME parsertest
    DEBUG_POSTFIX d )

install( TARGETS parser_test
    RUNTIME DESTINATION bin )

