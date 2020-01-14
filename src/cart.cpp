#include <cart.hpp>
#include <roller_coaster.hpp>
#include <model/rails/path.hpp>
#include <model/cart.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <model/tracks.hpp>

Cart Cart::behind(const Cart& cart_in_front) {
    // Copy the cart in front, but move one track ahead.
    Cart cart(cart_in_front);
    cart.track_num += 1;
    
    return cart;
}

Cart::Cart(const std::vector<TrackSegment>& track_sequence)
    : track_sequence(track_sequence)
    , track_num(0)
    , excess_distance(0.0f)
{
    this->init_buffers();
}

void Cart::init_buffers() {
    // Generate objects
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vertex_buffer);
    glGenBuffers(1, &this->index_buffer);

    // Fill the VAO with information about the buffers that are bound, and the format of said buffers
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


    // Fill buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);  // Vertices
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(CART_MODEL.vertices),
        CART_MODEL.vertices.elems,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, this->index_buffer);   // Indices
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(CART_MODEL.indices),
        CART_MODEL.indices.elems,
        GL_STATIC_DRAW
    );
}

void Cart::draw(ShaderProgram& sp) {
    // Bind the shader
    sp.bind();

    const auto pos = this->calc_position();
    const auto rot = this->calc_rotation();

    // Using our calculated pos and rot, generate matrices that perform those operations
    const auto translate = glm::translate(glm::mat4(1.0f), pos);
    const auto rotate = glm::rotate(rot.yaw, glm::vec3(0, -1, 0))
                * glm::rotate(rot.pitch, glm::vec3(1, 0, 0));
    
    // Combine operations into our model matrix
    const auto model = translate * rotate;
    
    // Set Model matrix in shader
    sp.set_uniform_mat4("model_matrix", model);

    // Finally draw cart
    glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, CART_MODEL.indices.size(), GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);
}

void Cart::update() {
    // What track type are we on? A slope? Straight? Curve?
    const auto track_type = this->track_sequence.at(this->track_num).type;

    // Depending on the type of track, move at a different speed
    const float distance_per_frame = track_type == TrackType::SlopeDown ? 0.045f
                                   : track_type == TrackType::SlopeUp ? 0.02f
                                   : 0.03f;

    const auto track_len = tracks::length(track_type);

    // Move the distance we calculated. 
    this->excess_distance += distance_per_frame;

    // If the distance is big enough, move to next segment
    if (this->excess_distance > track_len) {
        this->excess_distance -= track_len;
        this->track_num += 1;
        // Wrap the add so we loop around to start of track
        if (this->track_num == this->track_sequence.size()) {
            this->track_num = 0;
        }
    }
}

void Cart::update_behind(const Cart& cart_in_front) {
    // Updating a cart behind another, we use the cart in front's position
    // rather than calculating our own, and simply add an offset
    
    // The gap between us and the cart in front
    const float cart_gap = 1.2f;

    // Apply gap
    this->track_num = cart_in_front.track_num;
    this->excess_distance = cart_in_front.excess_distance - cart_gap;

    // Move between segments
    while (this->excess_distance < 0.0f) {

        this->track_num = this->track_num == 0 ? this->track_sequence.size() - 1 : this->track_num - 1;

        const auto track_type = this->track_sequence.at(this->track_num).type;
        const auto track_len = tracks::length(track_type);

        this->excess_distance = track_len + excess_distance;
    }
}


Rotation Cart::calc_rotation() const {
    const auto track_seg = this->track_sequence.at(this->track_num);
    const auto track_type = track_seg.type;
    const auto track_len = tracks::length(track_type);

    // What percentage are we thorugh the current track
    const auto percent = this->excess_distance / track_len;

    int previous_track_index = this->track_num - 1;
    if (previous_track_index < 0) previous_track_index = this->track_sequence.size() - 1;

    int next_track_index = this->track_num + 1;
    if (next_track_index >= this->track_sequence.size()) next_track_index = 0;

    // Get the types of the track in front and behind
    const auto prev_track_type = this->track_sequence.at(previous_track_index).type;
    const auto next_track_type = this->track_sequence.at(next_track_index).type;

    // Call the appropriate rotation function depending on what track we're on
    Rotation rot = tracks::calc_rotation(track_type)(percent, prev_track_type, next_track_type);

    // If the track is facing to the side at all, we need to account for that in our calculation
    if (track_seg.direction == Direction::Left) rot.yaw -= glm::radians(90.0f);
    if (track_seg.direction == Direction::Right) rot.yaw += glm::radians(90.0f);
    if (track_seg.direction == Direction::Down) rot.yaw += glm::radians(180.0f);

    return rot;
}

glm::vec3 Cart::calc_position() const {
    // Info about our current track
    const auto track_info = this->track_sequence.at(this->track_num);
    const auto track_type = track_info.type;
    const auto track_pos = track_info.position;
    const auto track_dir = track_info.direction;

    const auto track_len = tracks::length(track_type);

    // What percentage are we through the current track
    const auto percent = this->excess_distance / track_len;

    const glm::vec3 track_pos_f(track_pos.x, track_pos.y, track_pos.z);

    glm::vec3 track_offset = tracks::calc_position(track_type)(percent);

    // Depending on the tracks rotation, rotate our position
    if (track_dir == Direction::Left) rotate_vec_left(track_offset);
    if (track_dir == Direction::Right) rotate_vec_right(track_offset);
    if (track_dir == Direction::Down) rotate_vec_down(track_offset);

    // Our position is the tracks position, plus our offset within the track
    return track_pos_f + track_offset;
}
