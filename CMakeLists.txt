cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SnakeGame)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_PREFIX_PATH /usr/local/Cellar/sdl2_ttf/2.24.0)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)


set(SDL2_TTF_INCLUDE_DIR "/usr/local/Cellar/sdl2_ttf/2.24.0/include/SDL2")
set(SDL2_TTF_LIBRARY "/usr/local/Cellar/sdl2_ttf/2.24.0/lib/libSDL2_ttf.dylib")


message(STATUS "SDL2 Include Dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_TTF Include Dir: ${SDL2_TTF_INCLUDE_DIR}")
message(STATUS "SDL2_TTF Library: ${SDL2_TTF_LIBRARY}")

include_directories(${SDL2_INCLUDE_DIRS} src ${SDL2_TTF_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SOURCES
    src/main.cpp
    src/game.cpp
    src/snake.cpp
    src/astar.cpp
    src/renderer.cpp
    src/controller.cpp
)


add_executable(SnakeGame ${SOURCES})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARY})
