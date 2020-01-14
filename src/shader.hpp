#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class ShaderProgram;

enum class ShaderType {
    Vertex,
    Fragment,
};


// Turn our enum into the opengl constant
constexpr GLenum shader_type_constant(ShaderType type) {
    switch (type) {
        case ShaderType::Vertex:   return GL_VERTEX_SHADER;
        case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
    }
}


// Class for a shader. Note this is not a complete program, this stores either
// a vertex or fragment part. It is parameterised over the type at the type
// level
template<ShaderType type>
class Shader {
public:
    // Compile one from source
    Shader<type>(const char* source) {
        auto gl_constant = shader_type_constant(type);
        this->shader = glCreateShader(gl_constant);
        glShaderSource(this->shader, 1, &source, nullptr);
        glCompileShader(this->shader);

        // Check successful compilation
        int success;
        glGetShaderiv(this->shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char error_buf[512];
            glGetShaderInfoLog(this->shader, 512, nullptr, error_buf);
            std::cerr << "[ERROR]: Shader compiliation failed\n" << error_buf << std::endl;
        }
    };

    // Read file, then compile it from source
    static Shader<type> from_filepath(const char* filepath) {
        std::ifstream file_stream(filepath);
        std::stringstream buffer;
        buffer << file_stream.rdbuf();
        std::string source = buffer.str();

        return Shader<type>(source.c_str());
    }

    // Cleanup afterwards
    ~Shader<type>() {
        glDeleteShader(this->shader);
    }

    // Allow a total program to access the member variables
    friend class ShaderProgram;

private:
    // OpenGL handle for the program
    GLuint shader;
};

// Type aliases for convenience
using VertexShader = Shader<ShaderType::Vertex>;
using FragmentShader = Shader<ShaderType::Fragment>;
