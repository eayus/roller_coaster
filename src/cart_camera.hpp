#pragma once

#include <cart.hpp>
#include <glm/glm.hpp>

constexpr glm::vec3 CART_CAMERA_OFFSET = glm::vec3(0.0f, 0.8f, -0.4f);

class CartCamera {
public:
    CartCamera(const Cart& cart);
    glm::mat4 calculate_matrix() const;
    glm::vec3 get_position() const;

private:
    const Cart& cart;
};
