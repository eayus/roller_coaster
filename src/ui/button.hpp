#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

constexpr glm::vec3 MUSTARD_YELLOW = glm::vec3(1.0f, 0.86f, 0.35f);

struct UIVertex {
    glm::vec2 position;
    glm::vec3 color;
};

class Button {
public:
    Button(glm::vec2 center_pos, glm::vec2 size);
    void draw();
    bool is_in_bounds(glm::vec2 point);

private:
    GLuint vao;
    GLuint vertex_buffer;

    glm::vec2 center_pos;
    glm::vec2 size;
};
