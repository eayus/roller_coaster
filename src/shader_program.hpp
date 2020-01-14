#pragma once

#include <shader.hpp>
#include <glm/glm.hpp>

// Program combines a vertex and fragment shader.

class ShaderProgram {
public:
    // Construct a program using a vertex and fragment shader. We 
    ShaderProgram(VertexShader&& vshader, FragmentShader&& fshader);
    ~ShaderProgram();

    void bind();
    
    // Methods to set uniform variables
    void set_uniform_float(const char* name, float val);
    void set_uniform_vec3(const char* name, const glm::vec3& val);
    void set_uniform_mat4(const char* name, const glm::mat4& val);

private:
    GLuint program;
};
