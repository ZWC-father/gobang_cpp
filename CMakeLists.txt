cmake_minimum_required(VERSION 3.20)
project(GobangCpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()


find_package(SQLite3 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
include(FindPkgConfig)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(SOURCES
    src/main.cpp
    src/pve.cpp
    src/gui.cpp
    src/file.cpp
    src/judge.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_sdl3.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${GTK3_INCLUDE_DIRS}
)

add_executable(gobang_cpp ${SOURCES})

target_link_libraries(gobang_cpp
    PRIVATE
        SQLite::SQLite3
        OpenSSL::Crypto
	    OpenGL::GL
	    Threads::Threads
	    nfd
	    SDL3
	    ${GTK3_LIBRARIES}
)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_target_properties(gobang_cpp PROPERTIES
    BUILD_RPATH "/usr/local/lib"
    INSTALL_RPATH "/usr/local/lib"
)

message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
