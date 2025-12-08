cmake_minimum_required(VERSION 3.28)
project(JamGame LANGUAGES CXX C)

# 1. Global Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable folders for VS solution view
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 2. Dependencies (FetchContent)
include(FetchContent)

# --- SFML 3.0.2 ---
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

# --- Box2D (Latest) ---
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.0.0 
    GIT_SHALLOW ON
)
# Note: Box2D v3 changed API significantly.
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(box2d)

# --- GLAD (Local Vendor) ---
add_library(glad STATIC "vendor/glad/src/glad.c")
target_include_directories(glad PUBLIC "vendor/glad/include")

# 3. Game Executable
file(GLOB_RECURSE SOURCES 
    "src/*.cpp" 
    "src/*.h"
    "include/*.h" 
    "include/*.hpp"
    "src/UI/*.cpp"
    "src/UI/*.h"
)

add_executable(JamGame ${SOURCES})

# 4. Linking
target_link_libraries(JamGame PRIVATE SFML::Graphics SFML::Window SFML::System box2d glad)

# 5. Include Directories
# We add "${CMAKE_SOURCE_DIR}/src" so that if you have #include "Config.h" 
# inside a file, it finds it correctly inside the src folder.
target_include_directories(JamGame PRIVATE 
    include
    "${CMAKE_SOURCE_DIR}/src"
)

# 6. ASSET MANAGEMENT
# Copies the 'assets' folder to the build folder automatically
add_custom_command(TARGET JamGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying Assets to Binary Directory..."
)