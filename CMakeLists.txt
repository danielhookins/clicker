cmake_minimum_required(VERSION 3.10)
project(Clicker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the path to the custom cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Set the output directory for the build executables and libraries
set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2")
set(SDL2_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image")
set(SDL2_TTF_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf")
set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/include")
set(SDL2_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2.lib")
set(SDL2_DLL "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2.dll")
set(SDL2_MAIN_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2main.lib")
set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image/include")
set(SDL2_IMAGE_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image/lib/x64/SDL2_image.lib")
set(SDL2_TTF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf/include")
set(SDL2_TTF_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf/lib/x64/SDL2_ttf.lib")
set(SDL2_TTF_DLL "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf/lib/x64/SDL2_ttf.dll")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(Clicker src/main.cpp)

target_link_libraries(Clicker ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MAIN_LIBRARY})

# Copy SDL2.dll to the output directory after building
add_custom_command(TARGET Clicker POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${SDL2_DLL}
                   $<TARGET_FILE_DIR:Clicker>)

# Similarly, copy other required DLLs (e.g., SDL2_ttf.dll)
add_custom_command(TARGET Clicker POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${SDL2_TTF_DLL}
                   $<TARGET_FILE_DIR:Clicker>)
