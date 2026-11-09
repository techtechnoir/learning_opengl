#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
    // the program ID
    unsigned int ID;


public:
    // constructor reads and builds the shader
    Shader(const char* vertex_path, const char* fragment_path);

    // activate the shader
    void use();

    // utility uniform functions*************************************

    // universal set function hopefully it will work
    void set(const char* name, bool  value) const;
    void set(const char* name, int   value) const;
    void set(const char* name, float value) const;
    /*
    The first argument should be familiar by now which is the uniform's location.
    The second argument tells OpenGL how many matrices we'd like to send, which is 1.
    The third argument asks us if we want to transpose our matrix, that is
        to swap the columns and rows. OpenGL developers often use an internal matrix
        layout called column-major ordering which is the default matrix layout in GLM
        so there is no need to transpose the matrices; we can keep it at GL_FALSE.
    The last parameter is the actual matrix data, but GLM stores their matrices' data
        in a way that doesn't always match OpenGL's expectations so we first convert
        the data with GLM's built-in function glm::value_ptr.
    */
    void set(const char* name, unsigned int count, bool transpose, const float* value) const;


    // To get the id of the program unsigned int ID:
    // this->ID = glCreateProgram();
    unsigned int& get_program_id();

};





#endif  //SHADER_HPP