#######################################
# CMAKE guidolib
#######################################
cmake_minimum_required(VERSION 3.4)
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
set (STRVERS "v.1.7.0")
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
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
		set (CMAKE_OSX_ARCHITECTURES "arm64 armv7 armv7s")
		set (CMAKE_OSX_SYSROOT "iphoneos")
		set (libtype STATIC)
		set(IOS_DEPLOYMENT_TARGET 8.0) 
	else ()
		message (STATUS "Generates Mac OS project- Use -DIOS=yes to change.")
		set (CMAKE_C++_FLAGS -mmacosx-version-min=10.10)
#	set (CMAKE_MACOSX_RPATH 1)
		set (CMAKE_LDFLAGS -mmacosx-version-min=10.10)
		set (CMAKE_OSX_DEPLOYMENT_TARGET 10.10)
		set (MACOSX_DEPLOYMENT_TARGET 10.10)
	endif ()
endif()

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

	if (${CMAKE_GENERATOR} MATCHES ".*Win64")
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
# set includes
set(INCL ${INCL} "${SRC_PATH}/*")			# add include folders
set(INCL ${INCL} "${VGDEV_PATH}")			# add include folders
set(INCL ${INCL} "${MSLIGHT_PATH}")			# add include folders
file (GLOB INCDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${INCL})
include_directories( ${INCDIRS})
set_source_files_properties (${INCL} PROPERTIES HEADER_FILE_ONLY TRUE)

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
# describe targets
if (IOS)
	add_library(${target} STATIC ${SRC} ${HEADERS} ${PUBLICHEADERS})
	set_target_properties ( ${target} PROPERTIES 
			ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${IOS_DEPLOYMENT_TARGET} )

else()

	add_library(${target} SHARED ${SRC} ${HEADERS} ${PUBLICHEADERS})
	set_target_properties (${target} PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY  ${LIBDIR}
			RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
			FRAMEWORK TRUE
			FRAMEWORK_VERSION ${GUIDOFWKVERS}
			PUBLIC_HEADER "${PUBLICHEADERS}"
			VERSION ${VERSION}
			SOVERSION ${SOVERS}
			MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
			MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION} )

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


####################################
# add sub projects (tools)
####################################
if (IOS OR MIN)
	message (STATUS "Generates a minimal project (includes only the library)")
else()
	message (STATUS "Includes tools")
	add_subdirectory (tools)
	message (STATUS "Includes java jni")
	add_subdirectory (java)
	message (STATUS "Includes qt deployement process")
	add_subdirectory (postprocess)
endif()

if (NOT IOS)
#######################################
# install setup
#######################################
if (PACK)
set (CMAKE_INSTALL_PREFIX .)
endif()

message (STATUS "Library install prefix set to ${CMAKE_INSTALL_PREFIX}")
install ( TARGETS ${target}
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin CONFIGURATIONS Release
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib CONFIGURATIONS Release
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib CONFIGURATIONS Release
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX}/bin CONFIGURATIONS Release
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/guido
)

if (NOT MIN)
	####################################
	# install gmn files
	set (GMN ${ROOT}/gmn-examples)
	install (
		DIRECTORY ${GMN} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/guidolib
		FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
		DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
		FILES_MATCHING PATTERN "*.gmn"
	)

	####################################
	# install documentation
	set (DOC ${ROOT}/doc)
	install (
		DIRECTORY ${DOC}/html DESTINATION ${CMAKE_INSTALL_PREFIX}/share/guidolib/doc
		FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
		DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
		PATTERN ".DS_Store" EXCLUDE
		PATTERN "Makefile" EXCLUDE
	)
	file (GLOB PDF RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${DOC}/*.pdf  ${DOC}/refard/*.pdf ${DOC}/refard/latex/*.pdf )
	install (
		FILES     ${PDF} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/guidolib/doc
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	)
	install (
		FILES     ${DOC}/readme.md DESTINATION ${CMAKE_INSTALL_PREFIX}/share/guidolib/doc
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	)

	####################################
	# install guido font
	file (GLOB PDF RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${DOC}/*.pdf  ${DOC}/refard/*.pdf ${DOC}/refard/latex/*.pdf )
	install (
		FILES     ${ROOT}/src/guido2.ttf  ${ROOT}/src/guido2.svg  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/fonts/guido/guido2.ttf
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	)

	####################################
	# install python interface
	set (PYTHON ${ROOT}/lang/python)
	install (
		DIRECTORY ${PYTHON} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/guidolib
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
	if (WIN32)
	set (VS "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist/x64/Microsoft.VC140.CRT")
	#set (VS "${ROOT}/misc")
	install (
		FILES  "${VS}/msvcp140.dll" "${VS}/vcruntime140.dll" DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
		PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	)
	endif()

	####################################
	# install Qt applications
	set ( QT ${ROOT}/environments/Qt/apps)
	if (APPLE)
		set ( GUIDO2IMAGE guido2image)
		set ( GUIDOEDITOR GuidoEditor.app)
		set ( GUIDOSCENECOMPOSER GuidoSceneComposer.app)
		set ( QTAPPS ${QT}/guido2image/guido2image)
		set ( QTAPPS ${QT}/guido2image/${GUIDO2IMAGE})
	elseif(WIN32)
		set ( GUIDO2IMAGE guido2image.exe)
		set ( GUIDOEDITOR GuidoEditor.exe)
		set ( GUIDOSCENECOMPOSER GuidoSceneComposer.exe)
		set ( QTAPPS ${QT}/${GUIDO2IMAGE} ${QT}/${GUIDOEDITOR} ${QT}/${GUIDOSCENECOMPOSER} )
	elseif(UNIX)
		set ( QTAPPS ${QT}/guido2image/guido2image ${QT}/GuidoEditor/GuidoEditor ${QT}/GuidoSceneComposer/GuidoSceneComposer )
	endif()
	install (
		PROGRAMS ${QTAPPS}  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
		OPTIONAL
	)
	if (APPLE)
	set ( QTBUNDLES ${QT}/GuidoEditor/${GUIDOEDITOR} ${QT}/GuidoSceneComposer/${GUIDOSCENECOMPOSER})
	install (
		DIRECTORY ${QTBUNDLES} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
		USE_SOURCE_PERMISSIONS
		OPTIONAL
	)
	endif()
endif(NOT MIN)
endif(NOT IOS)


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
set (CPACK_PACKAGE_INSTALL_DIRECTORY  GuidoLib)

####################################
# NSIS specific section
set (CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files")
set (CPACK_NSIS_MODIFY_PATH on)
set (CPACK_NSIS_MUI_ICON "${ROOT}/misc/logo.ico")
set (CPACK_NSIS_EXTRA_INSTALL_COMMANDS  " ExecWait ./bin/vcredist_x64.exe")

include (CPack)
