cmake_minimum_required (VERSION 2.6)
project (guidohttpserver)
set (target guidohttpserver)
# LibFindMacros
include(../../cmake/LibFindMacros.cmake)

set (GUIDO  ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

FILE(GLOB SERVER_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../server/*.cpp)
FILE(GLOB SERVER_C_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../server/*.c)
FILE(GLOB CAIRO_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../src/Cairo*.cpp)
FILE(GLOB LOCAL_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/engine/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/engine/json)

add_definitions(-DJSON_ONLY)

# cairo
libfind_pkg_check_modules(cairo_PKGCONF cairo)
find_path(cairo_INCLUDE_DIR
  NAMES cairo.h
  PATHS ${cairo_PKGCONF_INCLUDE_DIRS}
)
find_library(cairo_LIBRARY
  NAMES cairo
  PATHS ${cairo_PKGCONF_LIBRARY_DIRS}
)
set(cairo_PROCESS_INCLUDES cairo_INCLUDE_DIR)
set(cairo_PROCESS_LIBS cairo_LIBRARY)
libfind_process(cairo)
add_definitions("-I" ${cairo_INCLUDE_DIRS})
set(LINK ${LINK} ${cairo_LIBRARIES})

# microhttpd
libfind_pkg_check_modules(microhttpd_PKGCONF libmicrohttpd)
find_path(microhttpd_INCLUDE_DIR
  NAMES microhttpd.h
  PATHS ${microhttpd_PKGCONF_INCLUDE_DIRS}
)
find_library(microhttpd_LIBRARY
  NAMES microhttpd
  PATHS ${microhttpd_PKGCONF_LIBRARY_DIRS}
)
set(microhttpd_PROCESS_INCLUDES microhttpd_INCLUDE_DIR)
set(microhttpd_PROCESS_LIBS microhttpd_LIBRARY)
libfind_process(microhttpd)
add_definitions("-I" ${microhttpd_INCLUDE_DIRS})
set(LINK ${LINK} ${microhttpd_LIBRARIES})

# crypto
libfind_pkg_check_modules(crypto_PKGCONF libcrypto)
find_path(crypto_INCLUDE_DIR
  NAMES openssl/sha.h
  PATHS ${crypto_PKGCONF_INCLUDE_DIRS}
)
find_library(crypto_LIBRARY
  NAMES crypto
  PATHS ${crypto_PKGCONF_LIBRARY_DIRS}
)
set(crypto_PROCESS_INCLUDES crypto_INCLUDE_DIR)
set(crypto_PROCESS_LIBS crypto_LIBRARY)
libfind_process(crypto)
add_definitions("-I" ${crypto_INCLUDE_DIRS})
set(LINK ${LINK} ${crypto_LIBRARIES})

# curl
libfind_pkg_check_modules(curl_PKGCONF libcurl)
find_path(curl_INCLUDE_DIR
  NAMES curl/curl.h
  PATHS ${curl_PKGCONF_INCLUDE_DIRS}
)
find_library(curl_LIBRARY
  NAMES curl
  PATHS ${curl_PKGCONF_LIBRARY_DIRS}
)
set(curl_PROCESS_INCLUDES curl_INCLUDE_DIR)
set(curl_PROCESS_LIBS curl_LIBRARY)
libfind_process(curl)
add_definitions("-I" ${curl_INCLUDE_DIRS})
set(LINK ${LINK} ${curl_LIBRARIES})

# Magick++
libfind_pkg_check_modules(Magick++_PKGCONF ImageMagick++)
add_definitions(${Magick++_PKGCONF_CFLAGS})
set(LINK ${LINK} ${Magick++_PKGCONF_LDFLAGS})
message(STATUS "Magick++ link flags: " ${Magick++_PKGCONF_LDFLAGS})

if(APPLE)
  set(LINK ${LINK}  "-framework GUIDOEngine  -F../../build/MacOS/Release")
else()
  set(LINK ${LINK}  "-l GUIDOEngine  -L../../build/unix/")
endif(APPLE)

add_executable(${target} ${LOCAL_CPP_SOURCES} ${SERVER_CPP_SOURCES} ${SERVER_C_SOURCES} ${CAIRO_CPP_SOURCES})
target_link_libraries(${target} ${LINK})
target_link_libraries(${target} ${GUIDOLIB})
