#pragma once

#include <GL/glew.h>
#include <shader_program.hpp>

// What direction to blur, and whether to blur at all
enum class Blur {
    Horizontal,
    Vertical,
    None,
};

// Class to render a texture to the entire screen

class ScreenTextureRenderer {
public:
    ScreenTextureRenderer();
    void draw_texture(Blur blur);       // Render a texture to the screen with some blur

private:
    // Different shaders for differnet blur effects
    ShaderProgram shader_prog;
    ShaderProgram blur_shader_horiz;
    ShaderProgram blur_shader_verti;

    // VAO and vertex buffer to store the screen quad.
    GLuint vao;
    GLuint vertex_buffer;
};
