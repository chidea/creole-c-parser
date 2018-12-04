#
# CMake file to create creole/c parser lib and DLL.
#

include_directories( ${creole_SOURCE_DIR}/include )

set( parser_SRC
     cwrapper.cpp
     parser.cpp
     lex.yy.c
     ConvertUTF.c )

set( parser_HEADERS
     "${creole_SOURCE_DIR}/include/creole.h"
     parser.h
     atom.h
     ConvertUTF.h )

set_source_files_properties( lex.yy.c PROPERTIES LANGUAGE CXX )

# CMake creates this source group only in the DLL project file for some reason :(
source_group( "Header Files" FILES ${parser_HEADERS} )

# static lib
add_library( creole_lib STATIC ${parser_SRC} )

set_target_properties( creole_lib PROPERTIES
    COMPILE_DEFINITIONS CREOLE_LIB
    OUTPUT_NAME creolelib
    DEBUG_POSTFIX d )

# DLL
add_library( creole_dll SHARED ${parser_SRC} ${parser_HEADERS} creole.rc )

set_target_properties( creole_dll PROPERTIES
    COMPILE_DEFINITIONS CREOLE_BUILDING
    OUTPUT_NAME creole
    VERSION 0.1.0
    SOVERSION 0
    DEBUG_POSTFIX d )

install( TARGETS creole_lib creole_dll
    RUNTIME DESTINATION bin 
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib )

