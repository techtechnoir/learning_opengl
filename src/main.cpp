#include "glad/glad.h"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

    Shader our_shader("../resources/shader.vs", "../resources/shader.fs");

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

        // shader program for the triangle
        our_shader.use();

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
        int vertex_color_location = glGetUniformLocation(our_shader.get_program_id(), "our_color");
        our_shader.set_float(vertex_color_location, green_value);

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
    glDeleteProgram(our_shader.get_program_id());

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