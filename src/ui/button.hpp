#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

constexpr glm::vec3 MUSTARD_YELLOW = glm::vec3(1.0f, 0.86f, 0.35f);

// A vertex used in rendering the simple 2D triangles used in the UI shader
struct UIVertex {
    glm::vec2 position;
    glm::vec3 color;
};

// A button class simply represents a rectangular object which we can easily
// determine whetehr a point is inside. It has no text or event handling,
// those are manually overlayed and combined

class Button {
public:
    Button(glm::vec2 center_pos, glm::vec2 size);
    void draw();
    bool is_in_bounds(glm::vec2 point); // Determine whether a point is inside the bounds. Useful for mouse clicks

private:
    GLuint vao;
    GLuint vertex_buffer;

    glm::vec2 center_pos;
    glm::vec2 size;
};
