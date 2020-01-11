#pragma once

#include <GL/glew.h>
#include <shader_program.hpp>

class ScreenTextureRenderer {
public:
    ScreenTextureRenderer();
    void draw_texture(GLuint texture);

private:
    ShaderProgram shader_prog;
    GLuint vao;
    GLuint vertex_buffer;
};
