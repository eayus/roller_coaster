#include <cart_camera.hpp>
#include <glm/gtx/rotate_vector.hpp>


CartCamera::CartCamera(const Cart& cart)
    : cart(cart) {}


glm::mat4 CartCamera::calculate_matrix() const {

    // What about if we always faced the track 2/3 steps ahead

    auto pos = this->cart.calc_position();
    auto rot = this->cart.calc_rotation();

    glm::mat4 rot_matrix = glm::rotate(rot.yaw, glm::vec3(0, -1, 0))
                         * glm::rotate(rot.pitch, glm::vec3(1, 0, 0));
    
    glm::vec3 facing = glm::vec3(rot_matrix * glm::vec4(0, 0, -1, 0));

    pos += glm::vec3(rot_matrix * glm::vec4(CART_CAMERA_OFFSET, 0.0f));

    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        1920.0f / 1080.0f,
        0.1f,
        1000.0f
    );

    glm::mat4 view = glm::lookAt(
        pos,
        pos + facing + glm::vec3(0.0f, -0.2f, 0.0f),
        glm::vec3(0, 1, 0)
    );

    return proj * view;
}


glm::vec3 CartCamera::get_position() const {
    auto pos = this->cart.calc_position();
    auto rot = this->cart.calc_rotation();

    glm::mat4 rot_matrix = glm::rotate(rot.yaw, glm::vec3(0, -1, 0))
                         * glm::rotate(rot.pitch, glm::vec3(1, 0, 0));

    pos += glm::vec3(rot_matrix * glm::vec4(CART_CAMERA_OFFSET, 0.0f));

    return pos;
}
