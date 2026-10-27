#include "glad/glad.h"
#include "shader.hpp"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Smt went wrong during GLAD loading!" << '\n';
        glfwTerminate();
        return 1;
    }
    // it is recommended that viewport should be set up before the first draw so that 
    // your first frame will be in correct size;
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Shader our_shader("../resources/shader.vs", "../resources/shader.fs");

    float vertices[] = {
        // positions          // colors            // texture coords
        0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  // top right
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // bot right
       -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // bot left
       -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // we can bind both EBO and VBO at the same time cause the former is element array buffer
    // where the latter is array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3 layouts in vertex shader, so 3 times vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (float*)vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // You can unbind firstly VBO and then VAO. You should not unbind EBO while VAO is bound
    // because EBO is directly a part of VAO, so you ll lose EBO info inside VAO.
    // EBO does not have something like glVertexAttribPointer that we do for VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    // load and create texture
    unsigned int texture;
    // first arg is the number of texture object we want to create
    glGenTextures(1, &texture);
    // all upcoming GL_TEXTURE_2D operations will now have effect on this texture obj.
    // uniform sampler2D a_texture1 inside fs, it corresponds to GL_TEXTURE0
    // so we do not call any uniform functions here but only activetexture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters (GL_REPEAT is default)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set the texture filtering parameters. remember we do not need mipmap for magnifying
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nr_channels;
    // the last param is the desired number of channel, 0 means preserve original nrchannel
    // 1 gray-scale, 2 gray-scale + alfa, 3 rgb, 4 rgba
    unsigned char* data = stbi_load("../resources/container.jpg", &width, &height, &nr_channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        /*
        - 1st argument specifies the texture target; setting this to GL_TEXTURE_2D means this
        operation will generate a texture on the currently bound texture object at the same target (so
        any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).
        - 2nd argument specifies the mipmap level for which we want to create a texture for if
        you want to set each mipmap level manually, but we’ll leave it at the base level which is 0.
        - 3rd argument tells OpenGL in what kind of format we want to store the texture. Our
        image has only RGB values so we’ll store the texture with RGB values as well.
        - 4th and 5th argument sets the width and height of the resulting texture. We stored those
        earlier when loading the image so we’ll use the corresponding variables.
        - 6th argument should always be 0 (some legacy stuff).
        - 7th and 8th argument specify the format and datatype of the source image. We loaded the
        image with RGB values and stored them as chars (bytes) so we’ll pass in the corresponding
        values.
        */
    }

    else
    {
        std::cout << "Failed to load texture!" << '\n';
        return 1;
    }

    // you may not need to unbind it
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // shader program for the textured rectangle
        our_shader.use();

        // bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // bind VAO and draw triangle with three vertices
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // unbind all of them to make it look more organized, you may not need tho
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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