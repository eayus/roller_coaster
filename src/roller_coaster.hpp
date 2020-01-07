#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <util.hpp>
#include <track.hpp>
#include <cart.hpp>

class RollerCoaster {
public:
    RollerCoaster(const char* filepath);
    const std::vector<glm::vec3>& get_path() const;
    void draw();

    Cart add_cart();

private:
    static void parse_track_sequence(const char* filepath, std::vector<TrackType>& track_sequence);
    void load_buffers(const std::vector<TrackType>& track_sequence);
    void calculate_path(const std::vector<TrackType>& track_sequence);
    

private:
    std::vector<TrackSegment> tracks;

    // OpenGL stuff
    GLuint vao;
    GLuint vertex_buffer;
    GLuint index_buffer;
    size_t num_indices;
};
