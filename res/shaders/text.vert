#version 330 core

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec2 v_tex_coord;

out vec2 f_tex_coord;

uniform float window_width;
uniform float window_height;

void main() {
    gl_Position = vec4(
        mix(-1, 1, v_position.x / window_width),
        mix(-1, 1, v_position.y / window_height),
        -0.5,
        1.0
    );

    f_tex_coord = v_tex_coord;
}
