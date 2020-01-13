#version 330 core

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec3 v_color;

out vec3 f_color;

uniform float window_width;
uniform float window_height;

void main() {
    gl_Position = vec4(
        mix(-1, 1, v_position.x / window_width),
        mix(-1, 1, v_position.y / window_height),
        -0.4,
        1.0
    );

    f_color = v_color;
}
