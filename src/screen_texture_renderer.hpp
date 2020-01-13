#pragma once

#include <GL/glew.h>
#include <shader_program.hpp>

enum class Blur {
    Horizontal,
    Vertical,
    None,
};

class ScreenTextureRenderer {
public:
    ScreenTextureRenderer();
    void draw_texture(Blur blur);

private:
    ShaderProgram shader_prog;
    ShaderProgram blur_shader_horiz;
    ShaderProgram blur_shader_verti;

    GLuint vao;
    GLuint vertex_buffer;
};
