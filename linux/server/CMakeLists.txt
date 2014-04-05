cmake_minimum_required (VERSION 2.6)
project (guidohttpserver)
set (target guidohttpserver)

set (GUIDO  ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

FILE(GLOB SERVER_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../server/*.cpp)
FILE(GLOB JSON_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../server/json/*.cpp)
FILE(GLOB SERVER_C_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../server/*.c)
FILE(GLOB CAIRO_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../src/Cairo*.cpp)
FILE(GLOB LOCAL_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../server/json)

add_definitions(-DJSON_ONLY)

# cairo
EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo"
        OUTPUT_VARIABLE TMP
        ERROR_VARIABLE TMP
)

string (STRIP ${TMP} CAIRO_INCL)
add_definitions(${CAIRO_INCL})
EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "cairo"
        OUTPUT_VARIABLE TMP
        ERROR_VARIABLE TMP
)
string (STRIP ${TMP} CAIRO_LIB)
set(LINK ${LINK} ${CAIRO_LIB})

# microhttpd
EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "libmicrohttpd"
        OUTPUT_VARIABLE TMP
        ERROR_VARIABLE TMP
)
string (STRIP ${TMP} MICROHTTPD_INCL)
add_definitions(${MICROHTTPD_INCL})
EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "libmicrohttpd"
        OUTPUT_VARIABLE TMP
        ERROR_VARIABLE TMP
)
string (STRIP ${TMP} MICROHTTPD_LIB)
set(LINK ${LINK} ${MICROHTTPD_LIB})

# openssl
EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "libcrypto"
        OUTPUT_VARIABLE TMP
        ERROR_VARIABLE TMP
)
string (STRIP ${TMP} CRYPTO_INCL)
add_definitions(${CRYPTO_INCL})
EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "libcrypto"
        OUTPUT_VARIABLE TMP
        ERROR_VARIABLE TMP
)
string (STRIP ${TMP} CRYPTO_LIB)
set(LINK ${LINK} ${CRYPTO_LIB})

# curl
EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "libcurl"
        OUTPUT_VARIABLE TMP
        ERROR_VARIABLE TMP
)
string (STRIP ${TMP} CURL_INCL)
add_definitions(${CURL_INCL})
EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "libcurl"
        OUTPUT_VARIABLE TMP
        ERROR_VARIABLE TMP
)
string (STRIP ${TMP} CURL_LIB)
set(LINK ${LINK} ${CURL_LIB})

if(APPLE)
  set(LINK ${LINK}  "-framework GUIDOEngine  -F../../build/MacOS/Release")
else()
  set(LINK ${LINK}  "-l GUIDOEngine  -L../../build/unix/")
endif(APPLE)

add_executable(${target} ${LOCAL_CPP_SOURCES} ${SERVER_CPP_SOURCES} ${SERVER_C_SOURCES} ${CAIRO_CPP_SOURCES} ${JSON_CPP_SOURCES})
target_link_libraries(${target} ${LINK})
target_link_libraries(${target} ${GUIDOLIB})
