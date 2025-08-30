#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/glad.h"
#include "utils.hpp"

enum shaderTypes
{
    VERTEX_SHADER = GL_VERTEX_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

class Shader
{
private:
    GLuint ProgramID;
    /// @brief This function reads the shader file and returns its content as a string.
    /// @param shader
    /// @param shader_type
    /// @param shaderFileName
    void compileShader(GLuint &shader, GLenum shader_type, const std::string shaderFileName);
    void compileProgram(GLuint vertex, GLuint fragment);

public:
    Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
    void use();
    GLuint getID();
    void setFloat(const std::string&, float) const;
    void setVec2(const std::string&, float, float) const;
    void setVec3(const std::string&, float, float, float) const;
    void setInt(const std::string& , int) const;
};

#endif