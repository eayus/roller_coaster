#include <cart_camera.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <globals.hpp>


CartCamera::CartCamera(const Cart& cart)
    : cart(cart) {}


glm::mat4 CartCamera::calculate_matrix() const {

    // Get the cart's location
    auto pos = this->cart.calc_position();
    auto rot = this->cart.calc_rotation();

    // Rotation matrix
    glm::mat4 rot_matrix = glm::rotate(rot.yaw, glm::vec3(0, -1, 0))
                         * glm::rotate(rot.pitch, glm::vec3(1, 0, 0));
    
    // Using the rotation matrix, find out which direction we face
    glm::vec3 facing = glm::vec3(rot_matrix * glm::vec4(0, 0, -1, 0));

    // Offset the camerea from the origin of cart. We have to rotate this, as cart may be angled
    pos += glm::vec3(rot_matrix * glm::vec4(CART_CAMERA_OFFSET, 0.0f));

    // Perspective projection
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(globals::WINDOW_DIMS.width) / static_cast<float>(globals::WINDOW_DIMS.height),
        0.1f,
        1000.0f
    );

    // Look at matrix. 
    glm::mat4 view = glm::lookAt(
        pos,
        pos + facing + glm::vec3(0.0f, -0.2f, 0.0f), // Look ahead from our current position
        glm::vec3(0, 1, 0)
    );

    // Come matrices
    return proj * view;
}


// Calculate our current position
glm::vec3 CartCamera::get_position() const {
    auto pos = this->cart.calc_position();
    auto rot = this->cart.calc_rotation();
    
    // We are simply an offset from the cart's position. Cart may be rotated, so we must rotate the offset too

    glm::mat4 rot_matrix = glm::rotate(rot.yaw, glm::vec3(0, -1, 0))
                         * glm::rotate(rot.pitch, glm::vec3(1, 0, 0));

    // Apply offset
    pos += glm::vec3(rot_matrix * glm::vec4(CART_CAMERA_OFFSET, 0.0f));

    return pos;
}
