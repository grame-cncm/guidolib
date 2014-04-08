cmake_minimum_required (VERSION 2.6)
project (GuidoOpenGL)
set (target GuidoOpenGL)
# LibFindMacros
include(../../../cmake/LibFindMacros.cmake)

set (GUIDO  ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/)

FILE(GLOB DEVICE_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../device/*.cpp)
FILE(GLOB APPLICATION_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../device/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../src/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

# gl
libfind_pkg_check_modules(gl_PKGCONF gl)
add_definitions(${gl_PKGCONF_CFLAGS})
set(LINK ${LINK} ${gl_PKGCONF_LDFLAGS})
message(STATUS "gl link flags: " ${gl_PKGCONF_LDFLAGS})

# glu
libfind_pkg_check_modules(glu_PKGCONF glu)
add_definitions(${glu_PKGCONF_CFLAGS})
set(LINK ${LINK} ${glu_PKGCONF_LDFLAGS})
message(STATUS "glu link flags: " ${glu_PKGCONF_LDFLAGS})

# glut doesn't have a pkg-config, so we cross our fingers that
# the directories with the opengl stuff have everything we need
set(LINK ${LINK} "-l glut")

# freetype
libfind_pkg_check_modules(freetype2_PKGCONF freetype2)
add_definitions(${freetype2_PKGCONF_CFLAGS})
set(LINK ${LINK} ${freetype2_PKGCONF_LDFLAGS})
message(STATUS "freetype2 link flags: " ${freetype2_PKGCONF_LDFLAGS})

# ftgl
libfind_pkg_check_modules(ftgl_PKGCONF ftgl)
add_definitions(${ftgl_PKGCONF_CFLAGS})
set(LINK ${LINK} ${ftgl_PKGCONF_LDFLAGS})
message(STATUS "ftgl link flags: " ${ftgl_PKGCONF_LDFLAGS})

if(APPLE)
  set(LINK ${LINK}  "-framework GUIDOEngine  -F../../../build/MacOS/Release")
else()
  set(LINK ${LINK}  "-l GUIDOEngine  -L../../../build/unix/")
endif(APPLE)

add_executable(${target} ${DEVICE_CPP_SOURCES} ${APPLICATION_CPP_SOURCES} ${SERVER_C_SOURCES} ${CAIRO_CPP_SOURCES} ${JSON_CPP_SOURCES})
target_link_libraries(${target} ${LINK})
target_link_libraries(${target} ${GUIDOLIB})
