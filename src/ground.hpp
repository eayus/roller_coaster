#pragma once

#include <GL/glew.h>
#include <roller_coaster.hpp>
#include <glm/glm.hpp>
#include <shader_program.hpp>

// Class representing the ground/grassy plane

constexpr glm::vec3 GRASS_COLOR = glm::vec3(0.37, 0.50, 0.22);
constexpr glm::vec3 STONE_COLOR = glm::vec3(0.533, 0.549, 0.553);
constexpr float GROUND_HEIGHT = 0.4f;
constexpr float EXTRA_RIM = 0.5f;

class Ground {
public:
    static Ground underneath_coaster(const RollerCoaster& rc); // Create a ground underneath a roller coaster
    void draw();

private:
    // OpenGL objects
    GLuint vao;
    GLuint vertex_buffer;
    GLuint index_buffer;
    size_t num_indices;
};
