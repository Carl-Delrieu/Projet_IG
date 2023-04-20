cmake_minimum_required(VERSION 3.10)

project(Projet_OpenGL)

# Stop if in the wrong place
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Source and build directories must be different!")
endif()

link_directories(${CMAKE_SOURCE_DIR}/lib)

# Find glfw
find_package(glfw3 3.3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# Create an executable
add_executable(test src/main.cpp src/common/glad.c)

# Link the executable to the libraries
target_link_libraries(test glfw)
target_include_directories(test PUBLIC include)

# Create examples
add_executable(curve example/bezier_curve.cpp src/common/glad.c)
target_link_libraries(curve glfw)
target_include_directories(curve PUBLIC include)

add_executable(curve_poly example/bezier_curve_poly.cpp src/common/glad.c)
target_link_libraries(curve_poly glfw)
target_include_directories(curve_poly PUBLIC include)


add_executable(surface example/bezier_surface.cpp src/common/glad.c)
target_link_libraries(surface glfw)
target_include_directories(surface PUBLIC include)

add_executable(surface_poly example/bezier_surface_poly.cpp src/common/glad.c)
target_link_libraries(surface_poly glfw)
target_include_directories(surface_poly PUBLIC include)

add_executable(surface_normals example/bezier_surface_normals.cpp src/common/glad.c)
target_link_libraries(surface_normals glfw)
target_include_directories(surface_normals PUBLIC include)

