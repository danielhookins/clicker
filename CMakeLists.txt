cmake_minimum_required(VERSION 3.10)
project(Clicker)

set(CMAKE_CXX_STANDARD 14)

# Add the path to the custom cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Set the output directory for the build executables and libraries
set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2")
set(SDL2_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image")
set(SDL2_TTF_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf")
set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/include")
set(SDL2_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2.lib")
set(SDL2_MAIN_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2main.lib")
set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image/include")
set(SDL2_IMAGE_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image/lib/x64/SDL2_image.lib")
set(SDL2_TTF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf/include")
set(SDL2_TTF_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf/lib/x64/SDL2_ttf.lib")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(Clicker src/main.cpp)

target_link_libraries(Clicker ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MAIN_LIBRARY})


