#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
}
)glsl";


const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
)glsl";


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LEARNING OPENGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // 3 vertices, each of which has 6 attributes (so stride is 6 float).
    float vertices[] = {
        // positions        // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Red
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Green
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Blue
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    /*
    A vertex array object (also known as VAO) can be bound just like a vertex buffer object and any
    subsequent vertex attribute calls from that point on will be stored inside the VAO. This has the
    advantage that when configuring vertex attribute pointers you only have to make those calls once
    and whenever we want to draw the object, we can just bind the corresponding VAO. This makes
    switching between different vertex data and attribute configurations as easy as binding a different
    VAO. All the state we just set is stored inside the VAO. A vertex array object stores the following:
        -Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
        -Vertex attribute configurations via glVertexAttribPointer.
        -Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.

    To use a VAO all you have to do is bind the VAO using glBindVertexArray. From that
    point on we should bind/configure the corresponding VBO(s) and attribute pointer(s) and then
    unbind the VAO for later use. As soon as we want to draw an object, we simply bind the VAO with
    the preferred settings before drawing the object and that is it.

    a VAO that stores our vertex attribute configuration and which VBO to use. Usually when you have multiple
    objects you want to draw, you first generate/configure all the VAOs (and thus the required VBO and
    attribute pointers) and store those for later use. The moment we want to draw one of our objects, we
    take the corresponding VAO, bind it, then draw the object and unbind the VAO again.
    */
    // bind the Vertex Array Object first, then 
    // bind and set vertex buffer(s), and then 
    // configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location 0)
    glVertexAttribPointer(
        0,                          // Attribute location in shader (layout location = 0)
        3,                          // Size (3 floats for x, y, z)
        GL_FLOAT,                   // Type
        GL_FALSE,                   // Normalize?
        6 * sizeof(float),          // Stride (total size per vertex: 6 floats), how many 
                                    // bytes it should skip to get from one vertex to the next vertex
        (void*)0                    // Offset (position starts at beginning of each vertex)
    );
    glEnableVertexAttribArray(0);   // Enable this attribute

    // Color attribute (location 1)
    glVertexAttribPointer(
        1,                          // Attribute location (layout location = 1)
        3,                          // Size (3 floats for r, g, b)
        GL_FLOAT,                   // Type
        GL_FALSE,                   // Normalize?
        6 * sizeof(float),          // Stride (still 6 floats per vertex)
        (void*)(3 * sizeof(float))  // Offset (color starts after 3 floats for position)
    );
    glEnableVertexAttribArray(1);   // Enable this attribute


    // note that this is allowed, the call to glVertexAttribPointer registered VBO 
    // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
    // but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways
    // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        process_input(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);

        /*
        To draw our objects of choice, OpenGL provides us with the glDrawArrays function that draws
        primitives using the currently active shader, the previously defined vertex attribute configuration and
        with the VBO’s vertex data (indirectly bound via the VAO).

        The glDrawArrays function takes as its first argument the OpenGL primitive type we would
        like to draw. Since I said at the start we wanted to draw a triangle, and I don’t like lying to you, we
        pass in GL_TRIANGLES. The second argument specifies the starting index of the vertex array we’d
        like to draw; we just leave this at 0. The last argument specifies how many vertices we want to draw,
        which is 3 (we only render 1 triangle from our data, which is exactly 3 vertices long).
        */
        // seeing as we only have a single VAO there's no need to bind it every time, 
        // but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
