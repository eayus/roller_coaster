#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <model/rails/path.hpp>
#include <vector>
#include <track.hpp>
#include <shader_program.hpp>

// Class representing a SINGLE cart that rides a roller coaster.

class Cart {
public:
    Cart(const std::vector<TrackSegment>& track_sequence); // Initialise a cart with the track it rides
    static Cart behind(const Cart& cart_in_front);         // Alternatively, initialise it behind another cart
    void update();                                         // Update a cart independently
    void update_behind(const Cart& cart_in_front);         // Update a cart which is behind another
    void draw(ShaderProgram& sp);                          // Draw the cart using the given shader
    glm::vec3 calc_position() const;                       // Calculate the position, using its location on the track
    Rotation calc_rotation() const;                        // Calculate the rotation, using its location on the track

private:
    // Fill the OpenGL buffers with the model
    void init_buffers();


    const std::vector<TrackSegment>& track_sequence;  // Store reference to the track
    size_t track_num;                                 // index into "track_sequence", which track segement we are oon
    float excess_distance;                            // For smooth movement, we can be halfway between two different tracks,
                                                      // the distance we have travelled in the current track is stored here.

    // OpenGL objects
    GLuint vao;
    GLuint vertex_buffer;
    GLuint index_buffer;
};
