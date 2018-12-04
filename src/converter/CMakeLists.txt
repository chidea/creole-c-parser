#
# CMake file for creole/c HTML converter.
#

include_directories( ${creole_SOURCE_DIR}/include )

link_directories( ${creole_BINARY_DIR}/lib )

set( converter_SRC converter.cpp main.cpp )
set( converter_HEADERS "${creole_SOURCE_DIR}/include/creole.h" converter.h )

source_group( "Header Files" FILES ${converter_HEADERS} )

add_executable( creole_convert
    ${converter_SRC} ${converter_HEADERS}
    creole.rc )

target_link_libraries( creole_convert creole_lib ) 

set_target_properties( creole_convert PROPERTIES
    COMPILE_DEFINITIONS CREOLE_LIB
    OUTPUT_NAME creole
    DEBUG_POSTFIX d )

install( TARGETS creole_convert
    RUNTIME DESTINATION bin )

