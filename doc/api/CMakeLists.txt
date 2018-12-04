#
# CMake file to create creole/c parser API doc
#

add_custom_target( api-docs doxygen ${creole_SOURCE_DIR}/doc/api/creole.dox
                   WORKING_DIRECTORY ${creole_SOURCE_DIR}/doc/api
		   COMMENT "Generating parser API documentation" )

