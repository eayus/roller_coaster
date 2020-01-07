#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_color;

out vec3 f_position;
out vec3 f_normal;
out vec3 f_color;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;

void main() {
    f_position = vec3(model_matrix * vec4(v_position, 1.0));
    f_normal = mat3(transpose(inverse(model_matrix))) * v_normal; // TODO: transform normals using model_matrix too
    f_color = v_color;
    gl_Position = camera_matrix * model_matrix * vec4(v_position, 1.0);
}
