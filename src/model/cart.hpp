#pragma once

#include <glm/glm.hpp>
#include <model/basic.hpp>
#include <model/transform.hpp>

// Function to generate the model for a single cart.

namespace cart {
    constexpr glm::vec3 COLOR = glm::vec3(0.35f, 0.35f, 0.35f);
    constexpr float WIDTH = 0.8f;
    constexpr float LENGTH = 0.8f;
    constexpr float HEIGHT = 0.5f;
    constexpr float THICKNESS = 0.05f;


    constexpr auto make_model() {
        float right_side = WIDTH * 0.5f;
        float left_side = -right_side;
        float front_side = LENGTH * 0.5f;
        float back_side = -front_side;

        auto surround = make_endless_cuboid(
            glm::vec3(left_side, 0.0f, front_side),
            glm::vec3(WIDTH, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -LENGTH),
            glm::vec3(0.0f, HEIGHT, 0.0f),
            COLOR
        );

        auto bottom = make_rect(
            glm::vec3(right_side, 0.0f, front_side),
            glm::vec3(-WIDTH, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -LENGTH),
            COLOR
        );
        
        auto inner = make_endless_cuboid(
            glm::vec3(left_side + THICKNESS, HEIGHT, front_side - THICKNESS),
            glm::vec3(WIDTH - (2.0f * THICKNESS), 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -LENGTH + (2.0f * THICKNESS)),
            glm::vec3(0.0f, -(HEIGHT - THICKNESS), 0.0f),
            COLOR
        );

        auto inner_bottom = make_rect(
            glm::vec3(left_side + THICKNESS, THICKNESS, front_side - THICKNESS),
            glm::vec3(WIDTH - (2.0f * THICKNESS), 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -(LENGTH - (2.0f * THICKNESS))),
            COLOR
        );

        auto left_rim = make_rect(
            glm::vec3(left_side, HEIGHT, front_side),
            glm::vec3(THICKNESS, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -LENGTH),
            COLOR
        );

        auto right_rim = make_rect(
            glm::vec3(right_side - THICKNESS, HEIGHT, front_side),
            glm::vec3(THICKNESS, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -LENGTH),
            COLOR
        );

        auto bottom_rim = make_rect(
            glm::vec3(left_side + THICKNESS, HEIGHT, front_side),
            glm::vec3(WIDTH - (2.0f * THICKNESS), 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -THICKNESS),
            COLOR
        );

        auto top_rim = make_rect(
            glm::vec3(left_side + THICKNESS, HEIGHT, back_side + THICKNESS),
            glm::vec3(WIDTH - (2.0f * THICKNESS), 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -THICKNESS),
            COLOR
        );

        return translate_model(
            combine_all(
                surround,
                bottom,
                inner,
                inner_bottom,
                left_rim,
                right_rim,
                bottom_rim,
                top_rim
            ),
            glm::vec3(0.0f, 0.2f, 0.0f)
        );
    }
}

constexpr auto CART_MODEL = cart::make_model();
