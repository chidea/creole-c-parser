#
# CMake file to create creole/c parser implementation doc
#

add_custom_target( impl-docs doxygen ${creole_SOURCE_DIR}/doc/impl/creole.dox
                   WORKING_DIRECTORY ${creole_SOURCE_DIR}/doc/impl
		   COMMENT "Generating parser implementation documentation" )

