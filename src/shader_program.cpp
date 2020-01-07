#include <shader_program.hpp>
#include <iostream>

ShaderProgram::ShaderProgram(VertexShader&& vshader, FragmentShader&& fshader) {
    this->program = glCreateProgram();

    glAttachShader(this->program, vshader.shader);
    glAttachShader(this->program, fshader.shader);

    glLinkProgram(this->program);

    // Check successful linking
    int success;
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success) {
        char error_buf[512];
        glGetProgramInfoLog(this->program, 512, nullptr, error_buf);
        std::cerr << "[ERROR]: Shader linking failed\n" << error_buf << std::endl;
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(this->program);
}

void ShaderProgram::bind() {
    glUseProgram(this->program);
}

void ShaderProgram::set_uniform_vec3(const char* name, const glm::vec3& val) {
    GLint loc = glGetUniformLocation(this->program, name);
    glUniform3fv(loc, 1, &val[0]);
}

void ShaderProgram::set_uniform_mat4(const char* name, const glm::mat4& val) {
    GLint loc = glGetUniformLocation(this->program, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
}
