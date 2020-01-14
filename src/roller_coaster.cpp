#include <GL/glew.h>
#include <roller_coaster.hpp>
#include <fstream>
#include <util.hpp>
#include <model/model.hpp>
#include <model/rails/straight.hpp>
#include <model/rails/curved.hpp>
#include <model/rails/path.hpp>
#include <model/rails/slope.hpp>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

constexpr size_t ESTIMATED_TRACK_VERTICES
    = (STRAIGHT_TRACK.vertices.size() + CURVED_TRACK.vertices.size()) / 2;

constexpr size_t ESTIMATED_TRACK_INDICES
    = (STRAIGHT_TRACK.indices.size() + CURVED_TRACK.indices.size()) / 2;


RCDims::RCDims(int min_x, int max_x, int min_z, int max_z)
    : min_x(min_x)
    , max_x(max_x)
    , min_z(min_z)
    , max_z(max_z) {}


RollerCoaster::RollerCoaster(const char* filepath) : dims(0, 0, 0, 0) {
    std::vector<TrackType> track_sequence;
    parse_track_sequence(filepath, track_sequence);

    // Generate buffers and arrays
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vertex_buffer);
    glGenBuffers(1, &this->index_buffer);

    glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    this->load_buffers(track_sequence);
    this->calculate_path(track_sequence);
}

Cart RollerCoaster::add_cart() {
    return Cart(this->tracks);
}

void RollerCoaster::draw() {
    glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);
}

void RollerCoaster::parse_track_sequence(const char* filepath, std::vector<TrackType>& track_sequence) {
    // Read the file, and process each line one by one
    std::ifstream file_stream(filepath);

    std::string line;
    while (getline(file_stream, line)) {
        track_sequence.push_back(*tracks::parse(line));
    }
}

void RollerCoaster::calculate_path(const std::vector<TrackType>& track_sequence) {
    this->tracks.reserve(track_sequence.size());

    // Position to place next track
    glm::ivec3 next_track_pos(0);
    Direction facing = Direction::Up; // Direction to place next track

    // Iterate through seuqence
    for (auto track : track_sequence) {
        // Calculate sizes
        if (next_track_pos.x < this->dims.min_x) this->dims.min_x = next_track_pos.x;
        if (next_track_pos.x > this->dims.max_x) this->dims.max_x = next_track_pos.x;
        if (next_track_pos.z < this->dims.min_z) this->dims.min_z = next_track_pos.z;
        if (next_track_pos.z > this->dims.max_z) this->dims.max_z = next_track_pos.z;

        // Calculate Path Stuff
        TrackSegment track_seg;
        track_seg.type = track;
        track_seg.position = next_track_pos;
        track_seg.direction = facing;

        this->tracks.push_back(track_seg);

        auto rel_fin = tracks::relative_finish(track);

        rotate_vec_by_dir(rel_fin.first, facing);
        next_track_pos += rel_fin.first;

        facing = concat_directions(facing, rel_fin.second);
    }
}

void RollerCoaster::load_buffers(const std::vector<TrackType>& track_sequence) {
    const size_t seq_len = track_sequence.size();

    // Create vectors, and reserve an estimated size, so we don't constantly realloc
    std::vector<Index> indices;
    indices.reserve(seq_len * ESTIMATED_TRACK_VERTICES);
    std::vector<Vertex> vertices;
    vertices.reserve(seq_len * ESTIMATED_TRACK_INDICES);

    glm::ivec3 next_track_pos(0);
    Direction facing = Direction::Up;

    unsigned int num_vertices;

    // Iterate sequence
    for (auto track : track_sequence) {
        auto model_data = tracks::model(track);

        num_vertices = vertices.size();

        //  Add every vertex in the track model
        for (size_t i = 0; i < model_data.vertices_len; i++) {
            Vertex vertex = model_data.vertices_start[i];

            rotate_vec_by_dir(vertex.position, facing);
            rotate_vec_by_dir(vertex.normal, facing);
            vertex.position += glm::vec3(next_track_pos);

            vertices.push_back(vertex);
        }


        // Add every index also
        for (size_t i = 0; i < model_data.indices_len; i++) {
            const Index index = model_data.indices_start[i];
            indices.push_back(index + num_vertices);
        }

        // Calculate the position for the next track
        auto rel_fin = tracks::relative_finish(track);
        rotate_vec_by_dir(rel_fin.first, facing);
        next_track_pos += rel_fin.first;

        facing = concat_directions(facing, rel_fin.second);
    }


    // Fill buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(Index),
        indices.data(),
        GL_STATIC_DRAW
    );

    this->num_indices = indices.size();
}


const RCDims& RollerCoaster::get_dims() const {
    return this->dims;
}
