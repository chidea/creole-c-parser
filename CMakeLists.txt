#
# CMake file to create creole/c parser and converter
#

cmake_minimum_required( VERSION 2.6 )

project( creole ) 

if( WIN32 )

  # avoid CRT security warnings
  add_definitions( -D_CRT_SECURE_NO_WARNINGS )
  
  # link against static CRT
  foreach( flag_var
		CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
		CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO )
    if( ${flag_var} MATCHES "/MD" )
      string( REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}" )
    endif( ${flag_var} MATCHES "/MD" )
  endforeach( flag_var )

  set( CMAKE_INSTALL_PREFIX "${creole_SOURCE_DIR}" )

endif( WIN32 )

add_subdirectory( src )
add_subdirectory( test )
add_subdirectory( doc )

