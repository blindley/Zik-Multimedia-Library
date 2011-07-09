project(Zik)
cmake_minimum_required(VERSION 2.6)

# package dependencies
find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL_LIBRARY})

# project source files
file (GLOB HEADER_FILES "*.h" "*.hpp")
file (GLOB SOURCE_FILES "*.cpp")

# build
set (CMAKE_DEBUG_POSTFIX "_d")
add_library(Zik ${SOURCE_FILES})
target_link_libraries(Zik ${LIBS})

# install
set (CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/../distribution")
install (TARGETS Zik DESTINATION bin/${CMAKE_GENERATOR})
install (FILES ${HEADER_FILES} DESTINATION include/Zik)
