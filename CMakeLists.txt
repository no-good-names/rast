cmake_minimum_required(VERSION 3.30)
project(rast C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/sdl EXCLUDE_FROM_ALL)
add_subdirectory(lib/cglm EXCLUDE_FROM_ALL)

if (UNIX AND NOT APPLE)
    list(APPEND LIBS m)
endif ()

set(LIBS m SDL3::SDL3 cglm)

add_executable(rast
	src/main.c
	src/backend.c
	src/backend.h
	src/primitives.c
	src/primitives.h
	src/utils/math.c
	src/utils/math.h
	src/utils/utils.c
	src/utils/utils.h)
target_link_libraries(rast PRIVATE ${LIBS})

