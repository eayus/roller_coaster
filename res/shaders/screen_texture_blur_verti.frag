#version 330 core

in vec2 f_tex_coord;

out vec4 color;

uniform sampler2D screen_texture;

void main() {
    float gaussian_weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    vec2 texel_size = 1.0 / textureSize(screen_texture, 0);

    vec3 result = texture(screen_texture, f_tex_coord).rgb * gaussian_weights[0];

    for (int i = 1; i < 5; i++) {
        result += texture(
            screen_texture,
            clamp(f_tex_coord + vec2(0.0, i * texel_size.y), 0.001, 0.999)
        ).rgb * gaussian_weights[i];

        result += texture(
            screen_texture,
            clamp(f_tex_coord + vec2(0.0, -i * texel_size.y), 0.001, 0.999)
        ).rgb * gaussian_weights[i];
    }

    //color = texture(screen_texture, f_tex_coord);
    color = vec4(result, 1.0);
}
