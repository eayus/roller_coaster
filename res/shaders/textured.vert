#version 330 core

layout (location = 0) in vec3 v_position;
//layout (location = 1) in vec2 v_texture_coords;

out vec2 f_texture_coords;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;

void main() {
    //f_position = vec3(model_matrix * vec4(v_position), 1.0);
    //f_normal = mat3(transpose(inverse(model_matrix))) * v_normal;

    f_texture_coords = v_position.xz;
    gl_Position = camera_matrix * model_matrix * vec4(v_position, 1.0);
}
