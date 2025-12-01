cmake_minimum_required(VERSION 3.14...3.22)
project(SpdlogExample)

# Add dependencies via CPM
include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME fmt
  GIT_TAG 10.2.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES"
)

CPMAddPackage(
  NAME spdlog
  GIT_TAG v1.14.1
  GITHUB_REPOSITORY gabime/spdlog
  OPTIONS "SPDLOG_INSTALL YES"
)

# Create example executable
add_executable(spdlog_example spdlog_example.cpp)

# Link libraries
target_link_libraries(spdlog_example PRIVATE fmt::fmt spdlog::spdlog)

# Include headers
target_include_directories(spdlog_example PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Set C++ standard
set_target_properties(spdlog_example PROPERTIES CXX_STANDARD 11)