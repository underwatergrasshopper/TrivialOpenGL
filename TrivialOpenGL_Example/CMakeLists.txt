cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("TrivialOpenGL_Example")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Example")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
# debug
#message("${SRC_FILES}")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")

target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/externals/TimeLapse/include")
target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory(.. TrivialOpenGL)
add_dependencies("${PROJECT_NAME}" TrivialOpenGL)

target_link_libraries("${PROJECT_NAME}" PUBLIC OpenGL32)
target_link_libraries("${PROJECT_NAME}" PUBLIC TrivialOpenGL)

### info ###

message(">>> ${PROJECT_NAME}: ${PROJECT_BINARY_DIR}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("FLAGS: ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("FLAGS: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()