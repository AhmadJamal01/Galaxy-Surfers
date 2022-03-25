#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle
        GLuint program;

    public:
        void create();
        void destroy();

        ShaderProgram() { program = 0; }
        ~ShaderProgram() { destroy(); }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use()
        {
            // TODO: call opengl to use the program identified by this->program
            glUseProgram(this->program);
        }

        GLuint getUniformLocation(const std::string &name)
        {
            // TODO: call opengl to get the uniform location for the uniform defined by name from this->program
            // the glGetUnifromLocation function takes in a glProgram and GLchar for the name of the uniform
            // I had to convert the string to a const char*
            // const char *name_char = name.c_str();
            return glGetUniformLocation(this->program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value)
        {
            // TODO: call opengl to set the value to the uniform defined by name
            // first get the uniform location using the getUniformLocation()
            // then set the value using glUniform1f() which takes the location and a single float value
            GLuint location = getUniformLocation(uniform);
            glUniform1f(location, value);
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            // TODO: call opengl to set the value to the uniform defined by name
            // this function and the next 2 should account for different vector sizes,
            // openGL uses glUniform*fv() to set uniforms with different vector sizes
            // the first parameter is the location of the uniform
            // the second parameter is the number of vectors
            // the third parameter is the value, which should also be converted from vec2 to float*
            glUniform2fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            // TODO: call opengl to set the value to the uniform defined by name
            glUniform3fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            // TODO: call opengl to set the value to the uniform defined by name
            glUniform4fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        // TODO: Delete the copy constructor and assignment operator
        // Question: Why do we do this? Hint: Look at the deconstructor
        ShaderProgram &operator=(ShaderProgram &) = delete;
        ShaderProgram(ShaderProgram &) = delete;
        // Answer: To avoid implicit generation of the copy constructors
        // usually when it does not make sense to have a copy of our class
        // or have two pointers pointing to the same location
        // perhaps also 2 shaders sharing the same id
    };

}

#endif