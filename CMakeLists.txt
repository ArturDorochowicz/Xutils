project( "Xutils" )

# Allow ENDIF() and ELSE() without expressions in parentheses
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )

message( STATUS "Generator: ${CMAKE_GENERATOR}" )


# Get current date (output is formatted as YYYY-MM-DD)
# Need to do this explicitly through shell, because neither EXEC_PROGRAM
# nor EXECUTE_PROCESS seem to use it.
exec_program( "$ENV{SystemRoot}/system32/cmd.exe" ARGS "/c date /t" OUTPUT_VARIABLE CURRENT_YEAR )
# leave year only
string( REGEX MATCH "[0-9]+" CURRENT_YEAR "${CURRENT_YEAR}" )


set( XUTILS_VERSION_MAJOR     "1" )
set( XUTILS_VERSION_MINOR     "0" )
set( XUTILS_VERSION_BUILD     "2" )
set( XUTILS_VERSION_REVISION  "0" )


add_subdirectory( "doc" )
add_subdirectory( "src" )



# Distribution target

set( DISTRIBUTION_DIR
	"${CMAKE_CURRENT_BINARY_DIR}/Xutils_v${XUTILS_VERSION_MAJOR}.${XUTILS_VERSION_MINOR}.${XUTILS_VERSION_BUILD}" )

get_target_property( XUTILS_LOCATION "Xutils" "LOCATION" )

add_custom_target( "Distribution"
	COMMAND
		"${CMAKE_COMMAND}"
			-E "make_directory" "${DISTRIBUTION_DIR}"
	COMMAND
		"${CMAKE_COMMAND}"
			-E "copy" "${CMAKE_CURRENT_BINARY_DIR}/doc/Xutils.html" "${DISTRIBUTION_DIR}"
	COMMAND
		"${CMAKE_COMMAND}"
			-E "copy" "${XUTILS_LOCATION}" "${DISTRIBUTION_DIR}" )

add_dependencies( "Distribution"
	"Xutils"
	"Documentation" )
