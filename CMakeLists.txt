cmake_minimum_required(VERSION 3.30)
project(rast C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

option(RAST_USE_OPENCL "Enable OpenCL support" ON)

add_subdirectory(lib/sdl EXCLUDE_FROM_ALL)
add_subdirectory(lib/cglm EXCLUDE_FROM_ALL)

set(RAST_LIBS "")
set(RAST_DEFS "")

if (UNIX AND NOT APPLE)
	list(APPEND RAST_LIBS m)
endif ()

list(APPEND RAST_LIBS SDL3::SDL3 cglm)

if (RAST_USE_OPENCL)
    find_package(OpenCL REQUIRED)
    list(APPEND RAST_LIBS OpenCL::OpenCL)
    list(APPEND RAST_DEFS CL_TARGET_OPENCL_VERSION=300 USE_OPENCL)
	message(WARNING "OpenCL is not yet (fully) implemented and might not function correctly")
endif()

add_executable(rast
	src/main.c
	src/backend.c
	src/backend.h
	src/primitives.c
	src/primitives.h
	src/native_gl.c
	src/native_gl.h
	src/math/math.c
	src/math/math.h
	src/math/transform.c
	src/math/transform.h
	src/utils/utils.c
	src/utils/utils.h)

target_link_libraries(rast PRIVATE ${RAST_LIBS})
target_compile_definitions(rast PRIVATE ${RAST_DEFS})

