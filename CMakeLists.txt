cmake_minimum_required(VERSION 3.28)

# --- MOVED THESE LINES TO THE TOP ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ------------------------------------

project(JamGame LANGUAGES CXX C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 2. Dependencies
include(FetchContent)

# SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git 
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

# Box2D
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.0.0 
    GIT_SHALLOW ON
)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(box2d)

# GLAD
add_library(glad STATIC "vendor/glad/src/glad.c")
target_include_directories(glad PUBLIC "vendor/glad/include")

# 3. Game Executable
# GLOB_RECURSE will find files in src/Core, src/Audio, etc. automatically
file(GLOB_RECURSE SOURCES 
    "src/*.cpp" 
    "src/*.h"
    "include/*.h" 
    "include/*.hpp"
)

add_executable(JamGame ${SOURCES})

# Ensure new entities and TOOLS are always built even if glob cache is stale
target_sources(JamGame PRIVATE
    src/Entities/Follower.cpp
    src/Entities/Follower.h
    src/Graphics/CharacterAnimator.cpp
    src/Graphics/CharacterAnimator.h
    src/Graphics/PlayerAnimator.cpp
    src/Graphics/PlayerAnimator.h
    src/Graphics/FollowerAnimator.cpp
    src/Graphics/FollowerAnimator.h
    
    # --- NEW TOOLS ADDED HERE ---
    src/Tools/Editor.cpp
    src/Tools/Editor.hpp
    src/Tools/GameObject.cpp
    src/Tools/GameObject.hpp
    src/Tools/ResourceManager.cpp
    src/Tools/ResourceManager.hpp
    src/Tools/CollisionUtils.hpp
    
    # --- ENSURE THESE ARE BUILT ---
    src/World/Map.cpp
    src/Props/Prop.cpp
)

# 4. Linking
target_link_libraries(JamGame PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio box2d glad)

# 5. Include Directories
# Added "src/Tools" so you can include "Editor.hpp" directly
target_include_directories(JamGame PRIVATE 
    include
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/Core"
    "${CMAKE_SOURCE_DIR}/src/Entities"
    "${CMAKE_SOURCE_DIR}/src/World"
    "${CMAKE_SOURCE_DIR}/src/Graphics"
    "${CMAKE_SOURCE_DIR}/src/Audio"
    "${CMAKE_SOURCE_DIR}/src/UI"
    "${CMAKE_SOURCE_DIR}/src/Props"
    "${CMAKE_SOURCE_DIR}/src/Tools"  # <--- ADDED THIS
)

# 6. Asset Management
add_custom_command(TARGET JamGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying Assets to Binary Directory..."
)