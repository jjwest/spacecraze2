cmake_minimum_required(VERSION 3.5)
project(spacecraze)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${spacecraze_SOURCE_DIR}/cmake")
set (CMAKE_BUILD_TYPE RELWITHDEBINFO)
set (CMAKE_CXX_FLAGS "-O2 -std=c++14 -Wall -Wextra -Woverloaded-virtual -pedantic -g")

file(GLOB SOURCES "src/*.cc" "src/*.c")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-lX11 -lpthread -lXrandr -lXi")

target_include_directories(
  ${CMAKE_PROJECT_NAME} PUBLIC
  ${SDL2_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR}
  ${SDL2_MIXER_INCLUDE_DIR}
  ${OPENGL_gl_INCLUDE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY}
                                ${SDL2_TTF_LIBRARIES}
                                ${SDL2_MIXER_LIBRARIES}
                                ${CMAKE_DL_LIBS}
                                ${OPENGL_gl_LIBRARY})
