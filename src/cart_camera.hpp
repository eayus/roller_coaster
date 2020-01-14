#pragma once

#include <cart.hpp>
#include <glm/glm.hpp>

// Camera position relative to the cart's origin
constexpr glm::vec3 CART_CAMERA_OFFSET = glm::vec3(0.0f, 0.8f, -0.4f);

// CartCamera is a camera following a cart.

class CartCamera {
public:
    CartCamera(const Cart& cart);        // Initialise it with the cart to flllow
    glm::mat4 calculate_matrix() const;  // Calculate the proj * view matrix
    glm::vec3 get_position() const;      // Get its current position

private:
    // The cart we're following
    const Cart& cart;
};
