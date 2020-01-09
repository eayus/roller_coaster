#pragma once

#include <GL/glew.h>
#include <roller_coaster.hpp>
#include <glm/glm.hpp>

constexpr glm::vec3 GRASS_COLOR = glm::vec3(0.37, 0.50, 0.22);

class Ground {
public:
    static Ground underneath_coaster(const RollerCoaster& rc);
    void draw();

private:
    GLuint vao;
    GLuint vertex_buffer;
    GLuint index_buffer;
};
