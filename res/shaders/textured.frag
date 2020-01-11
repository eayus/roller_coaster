#version 330 core

in vec2 f_texture_coords;

out vec4 color;

uniform sampler2D color_texture;

void main() {
    color = texture(color_texture, f_texture_coords);
}
