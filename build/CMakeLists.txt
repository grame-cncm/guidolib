#######################################
# CMAKE guidolib
#######################################
cmake_minimum_required(VERSION 3.5)
project(guidolib)

set (CMAKE_BUILD_TYPE Release)
set (target GUIDOEngine)
set (statictarget SGUIDOEngine)

get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

#######################################
# versions management
file (STRINGS "${ROOT}/version.txt" VERSION)
macro (get_major_minor_patch version)
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\1"  VERSION_MAJOR ${version} )
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\2"  VERSION_MINOR ${version} )
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\3"  VERSION_PATCH ${version} )
endmacro()
get_major_minor_patch( ${VERSION} )
set (SOVERS ${VERSION_MAJOR})
set (STRVERS "v.1.7.4")
set (GUIDOFWKVERS C)
message (STATUS "Configuring version ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

#######################################
# Options 
option ( IOS 		"Generates project for iOS" off )
option ( INDEPENDENTSVG "Independent SVG support (gets all font information from provided SVG files)" off)
option ( STATICLIB  "Generates a static library (default for iOS)" off)
option ( SMUFL  	"Use SMUFL compliant Bravura font" off)
option ( GL         "Links to opengl device" off)
option ( MIN        "Includes only the library in the project" off)
option ( MAKEFRAMEWORK  "Generates a MacOS framework" off)
option ( BUILDJNI   "Includes JNI library build" off)

#######################################
# Output folders 
set (BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set (LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)


set (CMAKE_CXX_STANDARD 11)

if(UNIX)
	set (DEFS -DGCC)
endif()

#########################################
# mac os specific part
#########################################
if(APPLE)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#######################################
#   iOS support
	if ( IOS )
		message (STATUS "Generates project for iOS - Use -DIOS=no to change.")
#		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
		set (CMAKE_OSX_ARCHITECTURES "arm64 armv7 armv7s")
		set (CMAKE_OSX_SYSROOT "iphoneos")
		set (CMAKE_LINK_LIBRARY_FLAG -fembed-bitcode)
		set (CMAKE_C_FLAGS -fembed-bitcode)
		set (CMAKE_CXX_FLAGS -fembed-bitcode)
		set (libtype STATIC)
		set (IOS_DEPLOYMENT_TARGET 10.0) 
	else ()
		message (STATUS "Generates Mac OS project- Use -DIOS=yes to change.")
#		set (CMAKE_CXX_FLAGS -mmacosx-version-min=12.00)
		set (CMAKE_LDFLAGS -mmacosx-version-min=10.13)
		set (CMAKE_OSX_DEPLOYMENT_TARGET 13.0)
#		set (MACOSX_DEPLOYMENT_TARGET 10.13)
	endif ()
endif()

set (CMAKE_BUILD_RPATH "../lib")
set (CMAKE_SKIP_BUILD_RPATH FALSE)


#########################################
# windows specific part
#########################################
if (WIN32)
	set( DEFS ${DEFS} GUIDOExport)
	if(MSVC)
		set( DEFS ${DEFS} MSVC )
	else()
		set( DEFS ${DEFS} GCC )
	endif()

	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set( DEFS ${DEFS} WIN64 )
		set (WIN64 1)
		set (target GUIDOEngine64)
		set (statictarget SGUIDOEngine64)
	endif ()
endif()


#########################################
# optional SMUFL support
#########################################
if (SMUFL)
	message (STATUS "Generates SMUFL compliant code.")
	set( DEFS "${DEFS} -DSMUFL" )
endif()


#######################################
# set directories, src and headers.
set (PLATFORMS ${ROOT}/platforms)
set (SRC_PATH  ${ROOT}/src/engine)
set (HEADERS_PATH ${SRC_PATH}/include)

if(WIN32)
	set(VGDEV_PATH ${PLATFORMS}/win32/src)
elseif(APPLE)
	set(VGDEV_PATH ${PLATFORMS}/macosx/src)
elseif(UNIX)
	set(VGDEV_PATH ${PLATFORMS}/linux/src)
endif()

#######################################
# independent svg support
if (INDEPENDENTSVG STREQUAL "yes")
	message (STATUS "Independent SVG will be enforced and all SVG fonts must be elaborated in SVG files - Use -DINDEPENDENTSVG=no to change.")
	set (DEFS "${DEFS} -DINDEPENDENTSVG" )
else()
	message (STATUS "Independent SVG will not be enforced and SVG fonts will be approximated using system fonts - Use -DINDEPENDENTSVG=yes to change.")
endif()


#######################################
# midi export support
set (MSLIGHT_PATH	${ROOT}/midisharelight/src)
set (DEFS ${DEFS} MIDIEXPORT MSLIGHT)

#######################################
# vgdev
include (cmake/vgdev.cmake)


#######################################
# set sources and headers files
file (GLOB SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SRC_PATH}/*/*.cpp" ${MSLIGHT_PATH}/*.cpp ${VGDEV_SRC})
file (GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SRC_PATH}/*/*.h" ${MSLIGHT_PATH}/*.h ${VGDEV_H})
file (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${HEADERS_PATH}/*.h" "${VGDEV_H}" "${SRC_PATH}/devices/*H")


#######################################
# set includes folders
file (GLOB INCDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  ${SRC_PATH}/* )
set (INCDIRS ${INCDIRS} ${VGDEV_PATH} ${MSLIGHT_PATH})

#######################################
# set library target
if(WIN32)
	enable_language(RC)
	set(SRC ${SRC} ${PLATFORMS}/win32/GUIDOEngine.rc)
	set(LINK ${LINK} "msimg32.lib" "gdiplus.lib")
endif(WIN32)

if(APPLE)
	set(LINK ${LINK} "-framework Carbon -framework CoreText")
elseif (UNIX)
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "cairo"   OUTPUT_VARIABLE TMP ERROR_VARIABLE TMP )
	string (STRIP ${TMP} CAIRO)
	set(LINK ${LINK} ${CAIRO})
endif()

#######################################
# targets description
if (IOS)
	add_library(${target} STATIC ${SRC} ${HEADERS} ${PUBLICHEADERS})
	target_include_directories (${target} PRIVATE ${INCDIRS})
	set_target_properties ( ${target} PROPERTIES 
			ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${IOS_DEPLOYMENT_TARGET} )

else()

	add_library(${target} SHARED ${SRC} ${HEADERS} ${PUBLICHEADERS})
	target_include_directories (${target} PRIVATE ${INCDIRS})
	set_target_properties (${target} PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY  ${LIBDIR}
			RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
			PUBLIC_HEADER "${PUBLICHEADERS}"
			VERSION ${VERSION}
			SOVERSION ${SOVERS}
	)
if (MAKEFRAMEWORK)
	set_target_properties (${target} PROPERTIES
			FRAMEWORK TRUE
			FRAMEWORK_VERSION ${GUIDOFWKVERS}
			MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
			MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION}
	)
endif()

	if (STATICLIB)
		message (STATUS "Static Guido lib will be generated - Use -DSTATICLIB=no to change.")
		add_library( ${statictarget} STATIC ${SRC} ${HEADERS} ${PUBLICHEADERS})
		target_compile_definitions (${statictarget} PRIVATE ${DEFS})
		target_include_directories (${statictarget} PRIVATE ${INCDIRS})
		set_target_properties(${statictarget} PROPERTIES 
			ARCHIVE_OUTPUT_DIRECTORY ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY ${LIBDIR})
	else ()
		message (STATUS "Static Guido lib will not be generated - Use -DSTATICLIB=yes to change.")
	endif ()
endif()



if(WIN32)
	set_target_properties (${target} PROPERTIES
		DEBUG_OUTPUT_NAME ${target}D
		RELEASE_OUTPUT_NAME ${target})
endif()

target_compile_definitions (${target} PRIVATE ${DEFS})
target_include_directories (${target} PRIVATE ${INCDIRS})
target_link_libraries (${target} ${LINK})


#######################################
# install paths 
# defined before loading subprojects
if (APPLE)
	set (CMAKE_INSTALL_PREFIX $ENV{HOME}/Guido-${VERSION})
elseif(WIN32)
	set (HOME $ENV{USERPROFILE})
	string (REPLACE "\\" "/" UXHOME "${HOME}")
	set (CMAKE_INSTALL_PREFIX ${UXHOME}/Guido-${VERSION})
endif()
if (PACK)
	set (CMAKE_INSTALL_PREFIX .)
elseif (DEFINED PREFIX)
	set (CMAKE_INSTALL_PREFIX ${PREFIX})
endif()

if (APPLE OR WIN32)
	set (INSTALL_BIN 		${CMAKE_INSTALL_PREFIX}/bin)
	set (INSTALL_LIB 		${CMAKE_INSTALL_PREFIX}/lib)
	set (INSTALL_INC 		${CMAKE_INSTALL_PREFIX}/dev/include)
	set (INSTALL_SAMPLES 	${CMAKE_INSTALL_PREFIX}/Examples)
	set (INSTALL_APPS		${CMAKE_INSTALL_PREFIX})
	set (INSTALL_DOC 		${CMAKE_INSTALL_PREFIX}/Documentation)
	set (INSTALL_INTERFACES ${CMAKE_INSTALL_PREFIX}/dev)
	set (INSTALL_FONTS 		${CMAKE_INSTALL_PREFIX}/Fonts)
else()
	set (INSTALL_BIN 		${CMAKE_INSTALL_PREFIX}/bin)
	set (INSTALL_LIB 		${CMAKE_INSTALL_PREFIX}/lib)
	set (INSTALL_INC 		${CMAKE_INSTALL_PREFIX}/include/guido)
	set (INSTALL_SAMPLES 	${CMAKE_INSTALL_PREFIX}/share/guidolib/examples)
	set (INSTALL_APPS 		${CMAKE_INSTALL_PREFIX}/bin)
	set (INSTALL_DOC 		${CMAKE_INSTALL_PREFIX}/share/guidolib/doc)
	set (INSTALL_INTERFACES ${CMAKE_INSTALL_PREFIX}/share/guidolib)
	set (INSTALL_FONTS ${CMAKE_INSTALL_PREFIX}/share/fonts/guidolib)
endif()


####################################
# add sub projects (tools)
####################################
if (IOS OR MIN)
	message (STATUS "Generates a minimal project (includes only the library)")
else()
	message (STATUS "Includes tools")
	add_subdirectory (tools)
	if (BUILDJNI)
		message (STATUS "Includes java jni")
		add_subdirectory (java)
	endif()
endif()


#######################################
# install setup
#######################################
if (IOS OR MIN)
	RETURN()
endif()

message (STATUS "Library install prefix set to ${CMAKE_INSTALL_PREFIX}")

install ( TARGETS ${target}
	RUNTIME DESTINATION ${INSTALL_BIN} CONFIGURATIONS Release
    LIBRARY DESTINATION ${INSTALL_LIB} CONFIGURATIONS Release
    ARCHIVE DESTINATION ${INSTALL_LIB} CONFIGURATIONS Release
    FRAMEWORK DESTINATION ${INSTALL_LIB} CONFIGURATIONS Release
    PUBLIC_HEADER DESTINATION ${INSTALL_INC}
)

install ( TARGETS ${target}
	RUNTIME DESTINATION ${INSTALL_APPS} CONFIGURATIONS Release
    LIBRARY DESTINATION ${INSTALL_LIB} CONFIGURATIONS Release
    ARCHIVE DESTINATION ${INSTALL_LIB} CONFIGURATIONS Release
    FRAMEWORK DESTINATION ${INSTALL_LIB} CONFIGURATIONS Release
    PUBLIC_HEADER DESTINATION ${INSTALL_INC}
)

####################################
# install tools
install (
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/ DESTINATION ${INSTALL_BIN}
	USE_SOURCE_PERMISSIONS
	PATTERN "Debug" EXCLUDE
)

####################################
# install gmn files
set (GMN ${ROOT}/gmn-examples/)
install (
	DIRECTORY ${GMN} DESTINATION ${INSTALL_SAMPLES}
	FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#	FILES_MATCHING PATTERN "*.gmn"
	PATTERN "SheetMusic" EXCLUDE
)

####################################
# install documentation
set (DOC ${ROOT}/doc)
install (
	DIRECTORY ${DOC}/html DESTINATION ${INSTALL_DOC}
	FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	PATTERN ".DS_Store" EXCLUDE
	PATTERN "Makefile" EXCLUDE
)
file (GLOB PDF RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	${DOC}/GUIDO-Music-Notation-Format.pdf  
	${DOC}/GUIDO_Add*.pdf  
	${DOC}/refcard/*.pdf 
	${DOC}/refcard/latex/*.pdf 
)
install (
	FILES     ${PDF} ${DOC}/readme.md DESTINATION ${INSTALL_DOC}
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

####################################
# install guido font
install (
	FILES     ${ROOT}/src/guido2.ttf  ${ROOT}/src/guido2.svg  DESTINATION ${INSTALL_FONTS}
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

####################################
# install python interface
set (PYTHON ${ROOT}/lang/python)
install (
	DIRECTORY ${PYTHON} DESTINATION ${INSTALL_INTERFACES}
	FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

####################################
# install readme
if (WIN32 OR APPLE)
install (
	FILES     ${ROOT}/misc/README.html ${ROOT}/changelog.txt DESTINATION ${CMAKE_INSTALL_PREFIX}
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
endif()

####################################
# install VS redistributables
if (MSVC)
  include (InstallRequiredSystemLibraries)
endif()

####################################
# install Qt applications
set ( QTBIN ${ROOT}/environments/Qt/apps/bin)
if (APPLE)
	file (GLOB QTAPPS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${QTBIN}/Guido*.app")
	file (GLOB QTTOOLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${QTBIN}/guido2image")
	install (
		DIRECTORY ${QTAPPS}  DESTINATION ${INSTALL_APPS}
		USE_SOURCE_PERMISSIONS
	)
elseif(WIN32)
	file (GLOB QTAPPS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${QTBIN}/Guido*.exe")
	file (GLOB QTTOOLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${QTBIN}/guido2image.exe")
	install ( PROGRAMS ${QTAPPS}  DESTINATION ${INSTALL_APPS})
elseif(UNIX)
	file (GLOB QTAPPS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${QTBIN}/Guido*")
	file (GLOB QTTOOLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${QTBIN}/guido2image")
	install (
		PROGRAMS ${QTAPPS}  DESTINATION ${INSTALL_APPS}
	)
endif()
install (
	PROGRAMS ${QTTOOLS}  DESTINATION ${INSTALL_BIN}
)
if (APPLE)
install ( CODE "message (STATUS \"Call macdeployqt\")"
		  CODE "execute_process ( COMMAND macdeployqt ${INSTALL_APPS}/GuidoEditor.app )"
		  CODE "execute_process ( COMMAND macdeployqt ${INSTALL_APPS}/GuidoSceneComposer.app )"
)
endif()

set (PACKNAME GuidoLib-${VERSION})
if(WIN32)
	if (PACK)
		set  (INSTALL_APPS ${CMAKE_CURRENT_SOURCE_DIR}/guidodir/_CPack_Packages/win64/NSIS64/${PACKNAME}-win64)
	endif()
	install ( CODE "message (STATUS \"Call windeployqt\")"
			  CODE "execute_process ( COMMAND windeployqt ${INSTALL_APPS}/GuidoEditor.exe )"
			  CODE "execute_process ( COMMAND windeployqt ${INSTALL_APPS}/GuidoSceneComposer.exe )"
	)
endif()


####################################
# cpack section
####################################
set (CPACK_PACKAGE_NAME   "GuidoLib")
set (CPACK_PACKAGE_VENDOR "GRAME")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "GuidoLib - a set of tools and libraries for music score rendering, based on the Guido Music Notation language.")
set (CPACK_PACKAGE_VERSION   ${VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR   ${VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR   ${VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH   ${VERSION_PATCH})

set (CPACK_MONOLITHIC_INSTALL true)
set (CPACK_RESOURCE_FILE_LICENSE  ${ROOT}/src/license.txt)
set (CPACK_PACKAGE_INSTALL_DIRECTORY  ${PACKNAME})
set (CPACK_GENERATOR NSIS64)


####################################
# NSIS specific section
set (CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files")
set (CPACK_NSIS_MODIFY_PATH on)
set (CPACK_NSIS_MUI_ICON "${ROOT}/misc/logo.ico")

include (CPack)
