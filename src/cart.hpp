#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <model/rails/path.hpp>
#include <vector>
#include <track.hpp>
#include <shader_program.hpp>

class Cart {
public:
    Cart(const std::vector<TrackSegment>& track_sequence);
    static Cart behind(const Cart& cart_in_front);
    void update();
    void update_behind(const Cart& cart_in_front);
    void draw(ShaderProgram& sp);
    glm::vec3 calc_position() const;
    Rotation calc_rotation() const;

private:
    void init_buffers();


    const std::vector<TrackSegment>& track_sequence;
    size_t track_num;
    float excess_distance;

    GLuint vao;
    GLuint vertex_buffer;
    GLuint index_buffer;
};
