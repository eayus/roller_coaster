#version 330 core

in vec2 f_tex_coord;

out vec4 color;

uniform sampler2D font_texture;

void main() {
    color = texture(font_texture, f_tex_coord);
}
