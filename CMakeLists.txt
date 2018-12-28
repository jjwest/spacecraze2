cmake_minimum_required(VERSION 3.0)
project(spacecraze)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${spacecraze_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -Woverloaded-virtual -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)


set(SOURCES
  src/audio.cc
  src/audio.h
  src/common.cc
  src/common.h
  src/entities.cc
  src/entities.h
  src/font.cc
  src/font.h
  src/game.cc
  src/game.h
  src/glad.c
  src/gui.cc
  src/gui.h
  src/main.cc
  src/mesh.cc
  src/mesh.h
  src/play_mode.cc
  src/play_mode.h
  src/renderer.cc
  src/renderer.h
  src/stb_image.cc
)

add_executable(spacecraze2 ${SOURCES})


target_include_directories(spacecraze2 PUBLIC
  ${SDL2_INCLUDE_DIR}
  ${SDL2_MIXER_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIRS}
  ${OPENGL_gl_INCLUDE_DIR}
  include
)

target_link_libraries(spacecraze2
  ${SDL2_LIBRARY}
  ${SDL2_MIXER_LIBRARIES}
  ${CMAKE_DL_LIBS}
  ${FREETYPE_LIBRARIES}
  ${OPENGL_gl_LIBRARY}
)
