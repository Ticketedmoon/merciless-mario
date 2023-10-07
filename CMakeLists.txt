cmake_minimum_required(VERSION 3.26.4)
project(merciless-mario)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(IS_RELEASE_MODE FALSE)

# Note: This compiler optimisation may break debugging.
if (IS_RELEASE_MODE)
    message("-- Loading release build, compiler optimisations set")
    set(CXXFLAGS  "-Wall -std=c++${CMAKE_CXX_STANDARD} -I./include -I./libs/headers -g -O2")
else()
    message("-- Loading debug build, compiler optimisations disabled")
    set(CXXFLAGS  "-Wall -std=c++${CMAKE_CXX_STANDARD} -I./include -I./libs/headers -g -O0")
endif ()
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

message("-- C++ Version Found " = " ${CMAKE_CXX_STANDARD}")

# The term "build type" in CMake refers to a certain build configuration such as debug and release.
# We give our configuration a debug build type by default.
# A debug build often results in fast compile times with debugging symbols baked
#  into executables - enabling debugging with build-tools as such as GDB and LLDB:

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()

# command generates a header file called config.h which includes preprocessor definitions that specify
#  our project's major and minor version number. CMake reads config.h.in to produce this header file:
# configure_file(config.h.in config.h)

file(GLOB ROOT_GAME_SOURCES "src/*.cpp")
file(GLOB CORE_GAME_SOURCES "src/core/*.cpp")
file(GLOB SCENE_SOURCES "src/scene/*.cpp")
file(GLOB ENTITY_SOURCES "src/entity/*.cpp")
file(GLOB SYSTEM_SOURCES "src/system/*.cpp")

file(GLOB MENU_SCENE_ROOT_COMPONENT_SOURCES "src/scene/menu_scene/*.cpp")
file(GLOB LEVEL_SELECT_SCENE_ROOT_COMPONENT_SOURCES "src/scene/level_select_scene/*.cpp")
file(GLOB GAME_OVER_SCENE_ROOT_COMPONENT_SOURCES "src/scene/game_over_scene/*.cpp")

file(GLOB GAMEPLAY_SCENE_ROOT_COMPONENT_SOURCES "src/scene/gameplay_scene/*.cpp")
file(GLOB GAMEPLAY_SCENE_COMPONENT_SOURCES "src/scene/gameplay_scene/component/*.cpp")
file(GLOB GAMEPLAY_SCENE_SYSTEM_SOURCES "src/scene/gameplay_scene/system/*.cpp")

file(COPY resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(${PROJECT_NAME} ${ROOT_GAME_SOURCES} ${CORE_GAME_SOURCES} ${SCENE_SOURCES} ${ENTITY_SOURCES} ${SYSTEM_SOURCES}
        ${MENU_SCENE_ROOT_COMPONENT_SOURCES} ${LEVEL_SELECT_SCENE_ROOT_COMPONENT_SOURCES} ${GAME_OVER_SCENE_ROOT_COMPONENT_SOURCES}
        ${GAMEPLAY_SCENE_ROOT_COMPONENT_SOURCES} ${GAMEPLAY_SCENE_COMPONENT_SOURCES} ${GAMEPLAY_SCENE_SYSTEM_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE include include/component include/core include/entity include/system
        include/scene include/scene/gameplay_scene include/scene/gameplay_scene/component
        include/scene/gameplay_scene/system  include/scene/menu_scene include/scene/menu_scene/system)

# Detect and add SFML
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
