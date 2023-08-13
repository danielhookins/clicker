cmake_minimum_required(VERSION 3.10)
project(Clicker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the path to the custom cmake modules (only if you have custom FindSDL2.cmake scripts, otherwise you can remove this)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Conditional check for Apple Silicon
if(APPLE AND CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
    include_directories(/opt/homebrew/include/SDL2)
    link_directories(/opt/homebrew/lib)
endif()

# Find SDL2 and its extensions
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(Clicker src/main.cpp)

if(APPLE AND CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
    target_link_libraries(Clicker ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} /opt/homebrew/lib/libSDL2_ttf.dylib)
else()
    target_link_libraries(Clicker ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # Copy SDL2.dll to the output directory after building (keep this if you plan to build on Windows too)
    add_custom_command(TARGET Clicker POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${SDL2_DLL}
                       $<TARGET_FILE_DIR:Clicker>)

    # Similarly, copy other required DLLs (e.g., SDL2_ttf.dll)
    add_custom_command(TARGET Clicker POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${SDL2_TTF_DLL}
                       $<TARGET_FILE_DIR:Clicker>)
endif()
