cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(GuidoQTLibraries)

option ( IOS    "Generate iOS libraries"	off )
option ( QT6    "Use Qt6"	on )

set (CMAKE_CXX_STANDARD 17)

#######################################
set (ROOT ${CMAKE_CURRENT_SOURCE_DIR})

#######################################
# GuidoQt settings
set (GuidoQt_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GuidoQt)
find_package(GuidoQt REQUIRED)

#######################################
# GraphicsSceneMainWindow settings
set (GRAPHICSCENE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GraphicsSceneMainWindow)
find_package(GRAPHICSCENE REQUIRED)

#######################################
if(IOS)
	message (STATUS "Generate libraries for iOS")
	set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iphoneos")
	set (CMAKE_OSX_SYSROOT "iphoneos")
endif()

#######################################
if(WIN32)
	# the following flags have beeen introduced for VS 16 2019
	set (CMAKE_CXX_FLAGS "-DWIN32 /EHsc /Zc:__cplusplus /permissive-")
endif()

#######################################
# set targets
if (IOS)
	set ( graphicscene GraphicsSceneMainWindow-iOS )
	set ( guidoqt GuidoQt-iOS )
else()
	set ( graphicscene GraphicsSceneMainWindow )
	set ( guidoqt GuidoQt )
endif()

if (UNIX AND NOT APPLE)
	set(CMAKE_CXX_FLAGS -fPIC)
endif()

set (LIBDIR ${CMAKE_CURRENT_SOURCE_DIR})

#######################################
# add GuidoQt
add_library(${guidoqt} STATIC ${GUIDOQT_SRC} ${GUIDOQT_HEADERS})
target_include_directories (${guidoqt} PRIVATE ${GUIDOQT_INCLUDE_DIRS})
if (DEFINED GUIDQOQT_DEFINITIONS)
	target_compile_definitions (${guidoqt} PRIVATE ${GUIDQOQT_DEFINITIONS})
endif()
set_target_properties (${guidoqt} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${LIBDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
	ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
	LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
)

#message ("GRAPHICSCENE_SRC ${GRAPHICSCENE_SRC}")
#######################################
# add GraphicsSceneMainWindow
add_library(${graphicscene} STATIC ${GRAPHICSCENE_SRC} ${GRAPHICSCENE_HEADERS})
target_include_directories (${graphicscene} PRIVATE ${GRAPHICSCENE_INCLUDE_DIRS})
if (DEFINED GRAPHICSCENE_DEFINITIONS)
	target_compile_definitions (${graphicscene} PRIVATE ${GRAPHICSCENE_DEFINITIONS})
endif()
set_target_properties (${graphicscene} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${LIBDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
	ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
	LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
)
