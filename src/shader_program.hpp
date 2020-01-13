#pragma once

#include <shader.hpp>
#include <glm/glm.hpp>

class ShaderProgram {
public:
    ShaderProgram(VertexShader&& vshader, FragmentShader&& fshader);
    ~ShaderProgram();

    void bind();
    void set_uniform_float(const char* name, float val);
    void set_uniform_vec3(const char* name, const glm::vec3& val);
    void set_uniform_mat4(const char* name, const glm::mat4& val);

private:
    GLuint program;
};
