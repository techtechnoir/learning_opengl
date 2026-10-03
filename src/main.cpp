/*
    TWO TRIANGLES NEXT TO EACH OTHER USING TWO VBOS/VAOS
    AND TWO VERTICES ARRAYS. BEWARE THE FIRST ARGUMENT OF
    glGenVertexArrays(2, VAOs) AND glGenBuffers(2, VBOs);
    AS WELL AS THEIR DELETING FUNCTIONS. IT IS 2 SINCE
    WE HAVE TWO-SIZED ARRAY OF INTEGERS (SHADERS) OTHERWISE,
    EVERYTHING SAME AS SINGLE TRIANGLE, INCLUDING SHADER SOURCES.
    XD
*/

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

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

void main()
{
    frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}

)glsl";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HELLYEAH", NULL, NULL);
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
        -0.9f, -0.4f, 0.0f,         // left  
        -0.1f, -0.4f, 0.0f,         // right 
        -0.5f,  0.4f, 0.0f,         // top
    };

    float vertices2[] = {
        -0.1f, -0.4f, 0.0f,         // left  
         0.7f, -0.4f, 0.0f,         // right 
         0.3f,  0.4f, 0.0f          // top
    };

    unsigned int VAO[2], VBO[2];
    glGenVertexArrays(2, &VAO[0]);
    glGenBuffers(2, &VBO[0]);

    // The 1st triangle
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (float*)vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0); // You may not need to unbind it (i ll tell in the next VAO binding)

    // The 2nd triangle
    // note that we bind to a different VAO now so we did not need to unbind VAOs[0]
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), (float*)vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // I ll unbind it cause i want to bind VAO[0] first in the loop

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // common shader program
        glUseProgram(shader_program);

        // bind VAO[0] and draw triangle with three vertices
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // bind VAO[1] and draw triangle with three vertices
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // unbind all of them to make it look more organized, you may not need tho
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, &VAO[0]);
    glDeleteBuffers(2, &VBO[0]);
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