PROJECT( "Xutils" )


MESSAGE( STATUS "Generator: ${CMAKE_GENERATOR}" )


# --- Output paths ---
SET( LIBRARY_OUTPUT_PATH
	"${CMAKE_BUILD_TYPE}" )
SET( EXECUTABLE_OUTPUT_PATH
	"${CMAKE_BUILD_TYPE}" )


# --- C compiler flags --- 
SET( CMAKE_C_FLAGS_RELEASE
	"/MD /O2" )
SET( CMAKE_C_FLAGS_MINSIZEREL
	"/MD /Os" )


# --- Find MS Platform SDK ---
FIND_PATH( PSDK_INCLUDE_DIR Windows.h
	"C:/Program Files/Microsoft Visual Studio 8/VC/PlatformSDK/Include"
	"C:/Program Files/Microsoft Platform SDK/Include" )

FIND_LIBRARY( PSDK_Kernel32_LIBRARY Kernel32
	"C:/Program Files/Microsoft Visual Studio 8/VC/PlatformSDK/Lib"
	"C:/Program Files/Microsoft Platform SDK/Lib" )

IF( PSDK_Kernel32_LIBRARY )
	GET_FILENAME_COMPONENT( PSDK_LIB_DIR "${PSDK_Kernel32_LIBRARY}" PATH )
ENDIF( PSDK_Kernel32_LIBRARY )

IF( PSDK_INCLUDE_DIR AND PSDK_LIB_DIR )
	MESSAGE( STATUS "Platform SDK Include dir: \"${PSDK_INCLUDE_DIR}\"" )
	MESSAGE( STATUS "Platform SDK Lib dir: \"${PSDK_LIB_DIR}\"" )
ELSE( PSDK_INCLUDE_DIR AND PSDK_LIB_DIR )
	MESSAGE( FATAL_ERROR "Platform SDK not found." )
ENDIF( PSDK_INCLUDE_DIR AND PSDK_LIB_DIR )


# --- Headers and libraries search directories ---
INCLUDE_DIRECTORIES( "${PSDK_INCLUDE_DIR}" )
LINK_DIRECTORIES( "${PSDK_LIB_DIR}" )


# --- Sources and libraries ---
SET( XUTILS_SOURCES
	"Xutils.c"
	"Eject.c"
	"IdleTimers.c"
	"RunAs.c"
	"Wheel.c"
	"resources.rc" )

SET( XUTILS_LIBRARIES
	"credui" )


# --- Targets ---

# --- Xutils.dll ---
SET( TARGET_SHARED_LIBRARY "Xutils" )
ADD_LIBRARY( ${TARGET_SHARED_LIBRARY} SHARED ${XUTILS_SOURCES} )
TARGET_LINK_LIBRARIES( ${TARGET_SHARED_LIBRARY} ${XUTILS_LIBRARIES} )
SET_TARGET_PROPERTIES( ${TARGET_SHARED_LIBRARY} PROPERTIES LINK_FLAGS_RELEASE "/opt:NOWIN98 /opt:REF /opt:ICF" )
SET_TARGET_PROPERTIES( ${TARGET_SHARED_LIBRARY} PROPERTIES LINK_FLAGS_MINSIZEREL "/opt:NOWIN98 /opt:REF /opt:ICF" )
