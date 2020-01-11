#version 330 core

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 v_tex_coord;

out vec2 f_tex_coord;

void main() {
    gl_Position = vec4(v_position, 0.0, 1.0);
    f_tex_coord = v_tex_coord;
}
