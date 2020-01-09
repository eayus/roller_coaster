#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <util.hpp>
#include <track.hpp>
#include <cart.hpp>

struct RCDims {
    RCDims(int min_x, int max_x, int min_z, int max_z);
    int min_x;
    int max_x;
    int min_z;
    int max_z;
};

class RollerCoaster {
public:
    RollerCoaster(const char* filepath);
    const std::vector<glm::vec3>& get_path() const;
    void draw();
    const RCDims& get_dims() const;

    Cart add_cart();

private:
    static void parse_track_sequence(const char* filepath, std::vector<TrackType>& track_sequence);
    void load_buffers(const std::vector<TrackType>& track_sequence);
    void calculate_path(const std::vector<TrackType>& track_sequence);
    

private:
    std::vector<TrackSegment> tracks;
    RCDims dims;

    // OpenGL stuff
    GLuint vao;
    GLuint vertex_buffer;
    GLuint index_buffer;
    size_t num_indices;
};
