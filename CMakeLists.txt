cmake_minimum_required(VERSION 3.10)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 11)

# Add your source files
add_executable(OpenGLTriangle
    src/main.cpp
    src/glad/glad.c
)

# Include directories (for glad)
target_include_directories(OpenGLTriangle PRIVATE include)

# Platform-specific linking
if (APPLE)
    # macOS: link OpenGL framework and GLFW
    find_package(glfw3 REQUIRED)
    target_link_libraries(OpenGLTriangle glfw "-framework OpenGL")
elseif (WIN32)
    # Windows: link against OpenGL32
    find_package(glfw3 REQUIRED)
    target_link_libraries(OpenGLTriangle glfw opengl32)
else()
    # Linux: link GL, dl, pthread, X11 as needed
    find_package(glfw3 REQUIRED)
    target_link_libraries(OpenGLTriangle glfw GL dl pthread X11)
endif()
