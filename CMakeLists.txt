cmake_minimum_required(VERSION 3.24)
project(tcc_2026_lib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/.deps)

# download progress
set(FETCHCONTENT_QUIET OFF CACHE BOOL "" FORCE)

# LSP commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CPM
file(
	DOWNLOAD
	https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
	${CMAKE_SOURCE_DIR}/.deps/cmake/CPM.cmake
	EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_SOURCE_DIR}/.deps/cmake/CPM.cmake)

# Dependencies
CPMAddPackage(
	NAME ortools
	GITHUB_REPOSITORY "google/or-tools"
	VERSION 9.15.6755
	GIT_TAG "v9.15"
	OPTIONS
	"BUILD_DEPS ON" 
	"BUILD_PYTHON OFF"
	"BUILD_JAVA OFF"
	"BUILD_DOTNET OFF"
	"BUILD_EXAMPLES OFF"
	"BUILD_SAMPLES OFF"
	"BUILD_TESTING OFF"
)

add_library(${PROJECT_NAME} STATIC
	src/optimization/ProblemData.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC src)

target_link_libraries(${PROJECT_NAME} PUBLIC ortools::ortools)

#add_executable(${PROJECT_NAME}_tests
#	tests/main.cpp
#	tests/repository/OrderRepository.cpp
#	tests/repository/MoldRepository.cpp
#)

#target_include_directories(${PROJECT_NAME}_tests PRIVATE src tests)

#target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME})
