#pragma once

#include <GL/glew.h>
#include <roller_coaster.hpp>
#include <glm/glm.hpp>
#include <shader_program.hpp>

constexpr glm::vec3 GRASS_COLOR = glm::vec3(0.37, 0.50, 0.22);
constexpr float EXTRA_RIM = 0.5f;

class Ground {
public:
    static Ground underneath_coaster(const RollerCoaster& rc);
    void draw();

private:
    GLuint vao;
    GLuint vertex_buffer;
    GLuint index_buffer;
    size_t num_indices;
};
