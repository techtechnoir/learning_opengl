#include "shader.hpp"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    // 1. retrieve the vertex/fragment source code from file_path
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;
    std::stringstream v_shader_stream;
    std::stringstream f_shader_stream;

    // ensure ifstream objects can throw exceptions:
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {   
        // open files
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);

        // read file's buffer contents into streams
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        //close file handlers
        v_shader_file.close();
        f_shader_file.close();

        // convert stream into string
        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    }

    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << '\n';
    }

    const char* v_shader_source = vertex_code.c_str();
    const char* f_shader_source = fragment_code.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_source, NULL);
    glCompileShader(vertex);

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << '\n';
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_source, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << '\n';
    }

    // shader program
    this->ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << '\n';
    }

    // delete shaders (we have our program)
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::set_bool(int location, bool value) const
{
    glUniform1i(location, value);
}
void Shader::set_int(int location, int value) const
{
    glUniform1i(location, value);
}

void Shader::set_float(int location, float value) const
{
    glUniform1f(location, value);
}

unsigned int& Shader::get_program_id()
{
    return this->ID;
}