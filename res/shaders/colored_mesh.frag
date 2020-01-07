#version 330 core

in vec3 f_position;
in vec3 f_normal;
in vec3 f_color;

out vec3 color;

uniform vec3 camera_position;

void main() {
    /* Requires that f_normal is normalized. */

    vec3 light_dir = normalize(vec3(0.3, 1.0, 0.175));    // light direction is "backwards"

    // Ambient
    float ambient = 0.3;

    // Diffuse
    float diffuse = 0.5 * max(dot(normalize(f_normal), light_dir), 0.0);

    // Specular
    vec3 camera_direction = normalize(camera_position - f_position);
    vec3 reflection_direction = reflect(-light_dir, normalize(f_normal));
    float specular = 0.2 * pow(max(dot(camera_direction, reflection_direction), 0.0), 16);
    

    float total = min(ambient + diffuse + specular, 1.0);
    color = f_color * total;
}
