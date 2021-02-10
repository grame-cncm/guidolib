cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(GuidoQTApplications)

#######################################
set (ROOT 		${CMAKE_CURRENT_SOURCE_DIR})
set (GUIDODIR  	${CMAKE_CURRENT_SOURCE_DIR}/../../..)

set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} guido2image)

#######################################
# shared guidoengine settings
set (GUIDO_INCLUDE_DIRS ${GUIDODIR}/src/engine/include)
if (WIN32)
	if ("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		set (GUIDO_LIBRARY ${GUIDODIR}/build/lib/GUIDOEngine64.lib)
	else()
		set (GUIDO_LIBRARY ${GUIDODIR}/build/lib/GUIDOEngine.lib)
	endif()
elseif (APPLE)
	set (GUIDO_LIBRARY "-F${GUIDODIR}/build/lib -framework GUIDOEngine")
else()
	set (GUIDO_LIBRARY "-L${GUIDODIR}/build/lib -lGUIDOEngine")
endif()

#message ("GUIDO_LIBRARY ${GUIDO_LIBRARY}")

#######################################
# guido2image settings
find_package(Guido2Image REQUIRED PATH ${CMAKE_CURRENT_SOURCE_DIR}/GuidoQt)

set (BINDIR ${ROOT}/bin)

#######################################
# add guido2image
set (g2i guido2image)
add_executable(${g2i} ${GUIDO2IMAGE_SRC} ${GUIDO2IMAGE_HEADERS})
target_include_directories (${g2i} PRIVATE ${GUIDO2IMAGE_INCLUDE_DIRS} ${GUIDO_INCLUDE_DIRS})
if (DEFINED GUIDO2IMAGE_DEFINITIONS)
	target_compile_definitions (${g2i} PRIVATE ${GUIDO2IMAGE_DEFINITIONS})
endif()
target_link_libraries (${g2i} PRIVATE ${GUIDO2IMAGE_LIBRARIES} ${GUIDO_LIBRARY})
set_target_properties (${g2i} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)

