#######################################
# CMAKE guidotools
#######################################
project(guidotools)
cmake_minimum_required(VERSION 2.6)
cmake_policy(VERSION 2.6)

set (CMAKE_BUILD_TYPE Release)

#######################################
# check for variables that are normally set at upper level
if (NOT DEFINED target)
	set (target GUIDOEngine)
endif()
if (NOT DEFINED ROOT)
	set (ROOT ../..)
endif()

#######################################
# set directories, src and headers.
set (GUIDOSRC	${ROOT}/src)
set (TOOLS_PATH	${GUIDOSRC}/tools)
set (TESTS_PATH	${ROOT}/validate)
set (GUIDO		${CMAKE_CURRENT_SOURCE_DIR}/../../..)

#######################################
if(UNIX)
	add_definitions(-Wall -DGCC)
endif(UNIX)

if(APPLE)
	set (DEVICE ${GUIDO}/platforms/macosx/src)
elseif(WIN32)
	set (DEVICE ${GUIDO}/platforms/win32/src)
elseif(UNIX)
	set (DEVICE ${GUIDO}/platforms/linux/src)
endif()

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
endif()

set (INCTOOLS ${GUIDOSRC}/engine/include ${GUIDOSRC}/engine/devices ${GUIDOSRC}/engine/tools ${DEVICE})

#######################################
# tools generation
set(TOOLS guidoparse guidoar2gr guidodraw)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TESTS_PATH}/${tool}.cpp )
	target_include_directories (${tool} PRIVATE ${INCTOOLS})
	target_link_libraries( ${tool} ${target})
	add_dependencies(${tool} ${target})
endforeach(tool)

set(TOOLS guido2midi guido2svg guido2abstract guido2proll midi2proll guido2svgmap guidogetversion guidogetpagecount guidogetvoicecount guidogettimemap guidogetpagemap guidogetstaffmap guidogetsystemmap guidogetvoicemap)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TOOLS_PATH}/${tool}.cpp )
	target_include_directories (${tool} PRIVATE ${INCTOOLS})
	target_link_libraries( ${tool} ${target})
	add_dependencies(${tool} ${target})
endforeach(tool)


#######################################
# install setup
install ( TARGETS ${TOOLS}
    	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)



