message( STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}" )

if( MSVC60 )
	if( NOT CMAKE_BUILD_TYPE STREQUAL "Distribution" )
		set( CMAKE_BUILD_TYPE "Distribution" )
		message( STATUS "Changed CMAKE_BUILD_TYPE to: ${CMAKE_BUILD_TYPE}" )
	endif( )

	set( CMAKE_C_FLAGS_DISTRIBUTION "/MD /O2 /Ob2 /G6 /GF" )
# /O2 maximize speed = /Ogityb1 /Gfy
# /O1 minimize size  = /Ogsyb1  /Gfy
# /Ob2 inline expantion - any suitable
# /GF enable read-only string pooling (overrides /Gf)
# /GR- disable RTTI
# /GX- disable C++ exception handling
# /Gi- disable incremental compilation
# /Gm- disable minimal rebuild
	set( LINK_FLAGS_DISTRIBUTION "/INCREMENTAL:NO /OPT:NOWIN98 /OPT:REF /OPT:ICF" )
endif( )

if( MSVC80 OR MSVC90 )
	add_definitions( "-D_CRT_SECURE_NO_DEPRECATE" )
endif( )

configure_file( "version.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/version.h" )
include_directories( BEFORE "${CMAKE_CURRENT_BINARY_DIR}" )

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include" )

set( SOURCES
	"Xutils.c"
	"Xutils.h"
	"Eject.c"
	"IdleTimers.c"
	"RunAs.c"
	"Wheel.c"
	"resources.rc"
	"version.h.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/version.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/powerpro.h" )

SET( LIBRARIES
	"credui" )

add_library( "Xutils" SHARED ${SOURCES} )
set_target_properties( "Xutils" PROPERTIES LINK_FLAGS_DISTRIBUTION "${LINK_FLAGS_DISTRIBUTION}" )
target_link_libraries( "Xutils" ${LIBRARIES} )
