#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 v_color;

//out vec3 f_color;

//uniform mat4 camera_matrix;

void main() {
    //gl_Position = camera_matrix * vec4(position, 1.0);
    //f_color = v_color;
    gl_Position = vec4(position, 1.0);
}
