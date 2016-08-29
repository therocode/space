#cmake ver req
cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   #for executables
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

#project name
set(project_name space)
project(${project_name})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Werror -Wshadow -Wconversion -Wno-long-long -pedantic -Wno-unused-parameter -Wno-unused-variable -Wno-format-pedantic")

file(GLOB_RECURSE source_files "src/*.cpp")
file(GLOB_RECURSE header_files "src/*.hpp")

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(executable_name "index")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s FULL_ES2=1 -Wno-gnu-anonymous-struct -Wno-nested-anon-types --preload-file data/")
else()
    set(executable_name ${project_name})
endif()

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
endif(OPENGL_FOUND)

find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
    include_directories(${OPENAL_INCLUDE_DIR})
endif(OPENAL_FOUND)

find_package(GLM REQUIRED)
if(GLM_FOUND)
    include_directories(${GLM_INCLUDE_DIRS})
endif()

find_package(Thero REQUIRED)
if(THERO_FOUND)
    include_directories(${THERO_INCLUDE_DIRS})
endif()

find_package(Featherkit COMPONENTS structure entity sdl2 audio rendering ui util REQUIRED)
if(FEATHERKIT_FOUND)
    include_directories(${FEATHERKIT_INCLUDE_DIRS})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DDEBUG_ON)
endif()

include_directories("json/src")
include_directories("src/data")

include_directories("imgui")
file(GLOB_RECURSE imgui_source_files "imgui/*.cpp")
file(GLOB_RECURSE imgui_header_files "imgui/*.h")

add_executable(${executable_name} ${header_files} ${source_files} ${imgui_header_files} ${imgui_source_files})
target_link_libraries(${executable_name} ${FEATHERKIT_LIBRARIES} ${OPENGL_LIBRARIES})
