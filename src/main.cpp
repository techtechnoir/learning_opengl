#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertex_shader_source = R"glsl(
#version 330 core
layout (location = 0) in vec3 a_pos;

void main()
{
    gl_Position = vec4(a_pos, 1.0);
}

)glsl";

const char* fragment_shader_source = R"glsl(
#version 330 core
out vec4 frag_color;

uniform vec4 our_color;

void main()
{
    frag_color = our_color;
}

)glsl";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LEARN OPENGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Smt went wrong during window creation!" << '\n';
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Smt went wrong during GLAD loading!" << '\n';
        glfwTerminate();
        return 1;
    }

    int success;
    char info_log[512];

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "Smt went wrong during vertex shader compiling: " << info_log << '\n';
    }

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "Smt went wrong during vertex shader compiling: " << info_log << '\n';
    }

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "Smt went wrong during program linking: " << info_log << '\n';
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    float vertices[] = {
         0.5f, -0.5f, 0.0f,         // left  
        -0.5f, -0.5f, 0.0f,         // right 
         0.0f,  0.5f, 0.0f,         // top
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (float*)vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0); // You may not need to unbind it

    glBindVertexArray(0); // I ll unbind it cause i want to bind VAO in the loop

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // shader program for the 1st triangle
        glUseProgram(shader_program);

        /*
        We query for the location of the our_color uniform using glGetUniformLocation.
        We supply the shader program and the name of the uniform to the query function.
        If glGetUniformLocation returns -1, it could not find the location. Lastly,
        we can set the uniform value using the glUniform4f function. Note that finding the
        uniform location does not require you to use the shader program first, but updating a
        uniform does require you to first use the program (by calling glUseProgram), because
        it sets the uniform on the currently active shader program.
        glUniform4f(location, r, g, b, a);
        */
        float time_value = glfwGetTime();
        float green_value = (sin(time_value) / 2.0f) + 0.5f;
        int vertex_color_location = glGetUniformLocation(shader_program, "our_color");
        glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

        // bind VAO and draw triangle with three vertices
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // unbind all of them to make it look more organized, you may not need tho
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}