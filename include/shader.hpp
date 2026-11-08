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


    // To get the id of the program unsigned int ID:
    // this->ID = glCreateProgram();
    unsigned int& get_program_id();

};





#endif  //SHADER_HPP