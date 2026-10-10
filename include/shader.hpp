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

    // You gotta use this function after getting the location of your
    // uniform by int loc = glGetUniformLocation(instance.ID, "name");
    void set_bool   (int location, bool value)   const;
    // You gotta use this function after getting the location of your
    // uniform by int loc = glGetUniformLocation(instance.ID, "name");
    void set_int    (int location, int value)    const;
    // You gotta use this function after getting the location of your
    // uniform by int loc = glGetUniformLocation(instance.ID, "name");
    void set_float  (int location, float value)  const;

    // To get the id of the program unsigned int ID:
    // this->ID = glCreateProgram();
    unsigned int& get_program_id();

};





#endif  //SHADER_HPP