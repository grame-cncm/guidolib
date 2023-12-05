#######################################
# CMAKE guidotools
#######################################
project(guidotools)
cmake_minimum_required(VERSION 3.5)

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
set (GUIDO		${CMAKE_CURRENT_SOURCE_DIR}/../..)

#######################################
if(UNIX)
	set (INCDEFS -DGCC)
	add_definitions(-Wall)
endif()

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

if (UNIX AND NOT APPLE)
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo"  OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} UINCL)
	set (INCDEFS "${INCDEFS} ${UINCL}")
endif()

#######################################
# tools generation
set(TOOLS guidoparse guidoar2gr guidodraw)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TESTS_PATH}/${tool}.cpp )
	target_include_directories (${tool} PRIVATE ${INCTOOLS})
	target_link_libraries( ${tool} ${target})
	set_target_properties(${tool}  PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINDIR})
	add_dependencies(${tool} ${target})
endforeach(tool)

set(TOOLS guido2midi guido2svg guido2abstract guido2proll midi2proll guido2svgmap guidogetversion guidogetpagecount guidogetvoicecount guidogettimemap guidogetpagemap guidogetstaffmap guidogetsystemmap guidogetvoicemap guidogettempi guidogetmeters guido2rproportional)
foreach(tool ${TOOLS})
	add_executable( ${tool} ${TOOLS_PATH}/${tool}.cpp )
	target_include_directories (${tool} PRIVATE ${INCTOOLS})
	target_compile_definitions (${tool} PRIVATE ${INCDEFS})
	target_link_libraries( ${tool} ${target})
	set_target_properties(${tool}  PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINDIR})
	add_dependencies(${tool} ${target})
endforeach(tool)



