cmake_minimum_required(VERSION 3.10)
project(pokemonpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add FTXUI
add_subdirectory(external/ftxui EXCLUDE_FROM_ALL)
include_directories(external/ftxui/include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(pokemon ${SOURCES})

# Include project headers
target_include_directories(pokemon PRIVATE include)

target_link_libraries(pokemon PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)


