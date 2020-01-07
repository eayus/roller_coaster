#include <cart.hpp>
#include <roller_coaster.hpp>
#include <model/rails/path.hpp>
#include <model/cart.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <model/tracks.hpp>

Cart Cart::behind(const Cart& cart_in_front) {
    // TODO: bug here which can make carts not space evenly.
    // We +1 onto track_num, but different tracks have
    // different lengths, and so carts will be spaced differently.
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


    // Fill buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(CART_MODEL.vertices),
        CART_MODEL.vertices.elems,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, this->index_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(CART_MODEL.indices),
        CART_MODEL.indices.elems,
        GL_STATIC_DRAW
    );
}

void Cart::draw(ShaderProgram& sp) {
    sp.bind();

    const auto pos = this->calc_position();
    const auto rot = this->calc_rotation();

    const auto translate = glm::translate(glm::mat4(1.0f), pos);
    const auto rotate = glm::rotate(rot.yaw, glm::vec3(0, -1, 0))
                * glm::rotate(rot.pitch, glm::vec3(1, 0, 0));
    
    const auto model = translate * rotate;
    
    sp.set_uniform_mat4("model_matrix", model);

    glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, CART_MODEL.indices.size(), GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);
}

void Cart::update() {
    const float distance_per_frame = 0.02f;

    const auto track_type = this->track_sequence.at(this->track_num).type;

    const auto track_len = tracks::length(track_type);

    this->excess_distance += distance_per_frame;
    if (this->excess_distance > track_len) {
        this->excess_distance -= track_len;
        this->track_num += 1;
        if (this->track_num == this->track_sequence.size()) {
            this->track_num = 0;
        }
    }
}


Rotation Cart::calc_rotation() const {
    const auto track_seg = this->track_sequence.at(this->track_num);
    const auto track_type = track_seg.type;

    const auto track_len = tracks::length(track_type);

    const auto percent = this->excess_distance / track_len;

    Rotation rot = tracks::calc_rotation(track_type)(percent);

    if (track_seg.direction == Direction::Left) rot.yaw -= glm::radians(90.0f);
    if (track_seg.direction == Direction::Right) rot.yaw += glm::radians(90.0f);
    if (track_seg.direction == Direction::Down) rot.yaw += glm::radians(180.0f);

    return rot;
}

glm::vec3 Cart::calc_position() const {
    const auto track_info = this->track_sequence.at(this->track_num);
    const auto track_type = track_info.type;
    const auto track_pos = track_info.position;
    const auto track_dir = track_info.direction;

    const auto track_len = tracks::length(track_type);

    const auto percent = this->excess_distance / track_len;

    const glm::vec3 track_pos_f(track_pos.x, track_pos.y, track_pos.z);

    glm::vec3 track_offset = tracks::calc_position(track_type)(percent);

    if (track_dir == Direction::Left) rotate_vec_left(track_offset);
    if (track_dir == Direction::Right) rotate_vec_right(track_offset);
    if (track_dir == Direction::Down) rotate_vec_down(track_offset);

    return track_pos_f + track_offset;
}
