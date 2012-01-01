#######################################
# CMAKE guidotools
#######################################
project(guidotools)
cmake_minimum_required(VERSION 2.6)

set (target GUIDOEngine)
#######################################
# set directories, src and headers.
set (TOOLS_PATH	${CMAKE_CURRENT_SOURCE_DIR}/..)
set (TESTS_PATH	${CMAKE_CURRENT_SOURCE_DIR}/../../validate)
set (GUIDO	${CMAKE_CURRENT_SOURCE_DIR}/../../src)

#######################################
if(UNIX)
	add_definitions(-Wall -DGCC)
endif(UNIX)

if(APPLE)
	add_definitions(-D_USE_QD_COORDINATES_)
	set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
	set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
	set (CMAKE_C++_FLAGS -mmacosx-version-min=10.4)
	set (CMAKE_LDFLAGS -mmacosx-version-min=10.4)
	set (DEVICE ${GUIDO}/../macosx/src)
elseif(WIN32)
	set (DEVICE ${GUIDO}/../win32/src)
elseif(UNIX)
	set (DEVICE ${GUIDO}/../linux/src)
endif()

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
endif()

include_directories( ${GUIDO}/include ${GUIDO}/devices ${GUIDO}/tools ${DEVICE} )

find_library (GUIDOLIB GUIDOEngine PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Release NO_DEFAULT_PATH)

#######################################
# tools generation
set(TOOLS guido2midi guido2svg guido2svgmap guidogetversion guidogetpagemap guidogetstaffmap guidogetsystemmap guidogetvoicemap)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TOOLS_PATH}/${tool}.cpp )
	target_link_libraries( ${tool} ${GUIDOLIB})
	add_dependencies(${tool} ${target})
endforeach(tool)

set(TOOLS guidoparse guidoar2gr guidodraw)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TESTS_PATH}/${tool}.cpp )
	target_link_libraries( ${tool} ${GUIDOLIB})
	add_dependencies(${tool} ${target})
endforeach(tool)

