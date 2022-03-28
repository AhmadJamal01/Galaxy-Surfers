#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

void our::ShaderProgram::create()
{
    // Create Shader Program
    program = glCreateProgram();
    /*
        Just like Buffer objects where we store data to be used in the GPU,
        a program object is used to specify a mechanism to link shaders and
        make use of them
    */
}

void our::ShaderProgram::destroy()
{
    // Delete Shader Program
    if (program != 0) // if the program is created successfuly
        glDeleteProgram(program);
    /*
        deleting a program frees the memory
        important notes:
        - if the program is being used, it will not be deleted till it's no longer in use
        - if the program to be deleted has shaders attached to it, the shaders will be detached
            but not deleted unless dlDeleteShader has been called on them.
        - if the program id passed is not corresponding to a valid program, a GL_INVALID_VALUE error
            is raised
    */
    program = 0;
}

// Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const
{
    // opengl wants shader source code as a string, a simple but not practical way
    // is to write the shader code in a string variable in our code
    // but it's a good practice to write shaders in separate files
    // and read those files and store their content in a string variable
    // below is a c++/c implementation to read file content into string variable

    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCStr = sourceString.c_str();
    file.close();

    // glCreateShader creates a shader object to maintain source code for this shader
    // it accepts the shader type as an argument which can be one of 3: VertexShader, FragmentShader, GeometryShader
    // it return a unique identifier that we can use to access this shader later
    GLuint shaderID = glCreateShader(type);

    // DONE  TODO: send the source code to the shader and compile it
    glShaderSource(shaderID, 1, &sourceCStr, NULL); // only COPIES the source code into the shader object
    glCompileShader(shaderID);
    // Here we check for compilation errors
    // TODO: Uncomment this if block
    if (std::string error = checkForShaderCompilationErrors(shaderID); error.size() != 0)
    {
        std::cerr << "ERROR IN " << filename << std::endl;
        std::cerr << error << std::endl;
        glDeleteShader(shaderID);
        return false;
    }

    // DONE  TODO: attach the shader to the program then delete the shader
    glAttachShader(program, shaderID);
    // glAttachShader specifies that the shader with shaderID identifier will be involved
    // in link operations for the specified program
    // Notes:
    // - shaders can be attached before adding/compiling source code
    // - same shader can be attached to multiple program objects
    // - multiple shaders can be attached to same program objects even if they have the same type

    glDeleteShader(shaderID);
    //  glDeleteShader deletes frees the memory but if the shader is being attached to a program
    // it will not be deleted immediately but till it's no longer attached to any valid program object
    // one way to detach a shader is by calling glDetachShader

    // We return true since the compilation succeeded
    return true;
}

bool our::ShaderProgram::link() const
{
    // DONE  TODO: call opengl to link the program identified by this->program
    glLinkProgram(program);
    // glLinkProgram links the program object and any attached shaders will be used to create executables
    // to run on the GPU
    // also all active-defined uniform variables will be set to 0 and assigned a location that
    // can be accessed using glGetUniformLocation
    // it will also install the executables to be used in the current rendering state if the program is in use

    // Here we check for linking errors
    // TODO: Uncomment this if block
    if (auto error = checkForLinkingErrors(program); error.size() != 0)
    {
        std::cerr << "LINKING ERROR" << std::endl;
        std::cerr << error << std::endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader)
{
    // Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program)
{
    // Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}