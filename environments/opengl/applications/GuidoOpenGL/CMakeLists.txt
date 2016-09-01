cmake_minimum_required (VERSION 2.6)
project (GuidoOpenGL)
set (target GuidoOpenGL)

set (ROOT  ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
set (GUIDO ${ROOT}/src/engine)

FILE(GLOB DEVICE_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../device/*.cpp)
FILE(GLOB APPLICATION_CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../device/)
include_directories(${GUIDO}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

if(APPLE)
	add_definitions(-DAPPLE )
elseif(WIN32)
	add_definitions(-DWIN32 )
else()
	add_definitions(-DLINUX )
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  message(STATUS "opengl found")
  include_directories(${OPENGL_INCLUDE_DIR})
  set(LINK ${LINK} ${OPENGL_LIBRARY})
endif ()

find_package(GLUT REQUIRED)
if (GLUT_FOUND)
  message(STATUS "glut found")
  include_directories(${GLUT_INCLUDE_DIR})
  set(LINK ${LINK} ${GLUT_LIBRARY})
endif ()

# freetype and ftgl
include_directories(/usr/local/include/freetype2 /usr/local/include)
set(LINK ${LINK} -L/usr/local/lib -lfreetype -lftgl)

if(APPLE)
  set(LINK ${LINK}  "-F${ROOT}/build -framework GUIDOEngine")
else()
  set(LINK ${LINK}  "-L${ROOT}/build/unix/ -l GUIDOEngine")
endif()

add_executable(${target} ${DEVICE_CPP_SOURCES} ${APPLICATION_CPP_SOURCES} ${SERVER_C_SOURCES} ${CAIRO_CPP_SOURCES} ${JSON_CPP_SOURCES})
target_link_libraries(${target} ${LINK})
target_link_libraries(${target} ${GUIDOLIB})
