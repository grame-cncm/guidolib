cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(GuidoQTApplications)

option ( QT6    "Use Qt6"	on )
set (CMAKE_CXX_STANDARD 17)

#######################################
set (ROOT 		${CMAKE_CURRENT_SOURCE_DIR})
set (GUIDODIR  	${CMAKE_CURRENT_SOURCE_DIR}/../../..)

set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} guido2image GuidoEditor GuidoSceneComposer)

#######################################
# shared guidoengine settings
set (GUIDO_INCLUDE_DIRS ${GUIDODIR}/src/engine/include)
if (WIN32)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set (GUIDO_LIBRARY ${GUIDODIR}/build/lib/GUIDOEngine64.lib)
	else()
		set (GUIDO_LIBRARY ${GUIDODIR}/build/lib/GUIDOEngine.lib)
	endif()
elseif (APPLE)
	set (GUIDO_LIBRARY "-L${GUIDODIR}/build/lib -lGUIDOEngine")
else()
	set (GUIDO_LIBRARY "-L${GUIDODIR}/build/lib -lGUIDOEngine")
endif()

set (CMAKE_BUILD_RPATH "${GUIDODIR}/build/lib")
set (CMAKE_SKIP_BUILD_RPATH FALSE)

#######################################
# guido2image settings
find_package(Guido2Image REQUIRED )

#######################################
# GuidoEditor settings
find_package(GuidoEditor REQUIRED )

#######################################
# GuidoSceneComposer settings
find_package(GuidoSceneComposer REQUIRED )

set (BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

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
	INSTALL_RPATH ${GUIDODIR}/build/lib
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)

#######################################
# add GuidoEditor
set (guidoeditor GuidoEditor)
add_executable(${guidoeditor} ${GUIDOEDITOR_EXE} ${GUIDOEDITOR_SRC} ${GUIDOEDITOR_HEADERS})
target_include_directories (${guidoeditor} PRIVATE ${GUIDOEDITOR_INCLUDE_DIRS} ${GUIDO_INCLUDE_DIRS})
if (DEFINED GUIDOEDITOR_DEFINITIONS)
	target_compile_definitions (${guidoeditor} PRIVATE ${GUIDOEDITOR_DEFINITIONS})
endif()
target_link_libraries (${guidoeditor} PRIVATE ${GUIDOEDITOR_LIBRARIES} ${GUIDO_LIBRARY})
set_target_properties (${guidoeditor} PROPERTIES
	BUNDLE true
	MACOSX_BUNDLE_BUNDLE_NAME ${guidoeditor}
	MACOSX_BUNDLE_ICON_FILE ${GUIDOEDITOR_ICON_FILE}
	MACOSX_BUNDLE_BUNDLE_VERSION "v.${VERSION}"
	MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}"
	MACOSX_BUNDLE_INFO_PLIST ${GUIDOEDITOR_INFO_PLIST}
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)

#######################################
# add GuidoSceneComposer
set (guidosc GuidoSceneComposer)
add_executable(${guidosc} ${GUIDOSCENECOMPOSER_EXE} ${GUIDOSCENECOMPOSER_SRC} ${GUIDOSCENECOMPOSER_HEADERS})
target_include_directories (${guidosc} PRIVATE ${GUIDOSCENECOMPOSER_INCLUDE_DIRS} ${GUIDO_INCLUDE_DIRS})
target_compile_definitions (${guidosc} PRIVATE ${GUIDOSCENECOMPOSER_DEFINITIONS})
target_link_libraries (${guidosc} PRIVATE ${GUIDOSCENECOMPOSER_LIBRARIES} ${GUIDO_LIBRARY})
set_target_properties (${guidosc} PROPERTIES
	INSTALL_RPATH ${GUIDODIR}/build/lib
	BUNDLE true
	MACOSX_BUNDLE_BUNDLE_NAME ${guidosc}
	MACOSX_BUNDLE_ICON_FILE ${GUIDOSCENECOMPOSER_ICON_FILE}
	MACOSX_BUNDLE_BUNDLE_VERSION "v.${VERSION}"
	MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}"
	MACOSX_BUNDLE_INFO_PLIST ${GUIDOSCENECOMPOSER_INFO_PLIST}
	RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR}
)


