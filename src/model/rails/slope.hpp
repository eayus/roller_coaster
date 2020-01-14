#pragma once

#include <model/basic.hpp>
#include <model/transform.hpp>
#include <model/rails/config.hpp>
#include <gcem/gcem.hpp>

// Functions to generate the model for the sloped track.

namespace sloped {
    enum class Side {
        Left,
        Right,
    };

    constexpr auto make_rail(Side side) {
        float right_edge = TRACK_WIDTH / 2.0f;
        float left_edge = -right_edge;
        float hrt = RAIL_THICKNESS / 2.0f;
        float rt_component = RAIL_THICKNESS / gcem::sqrt(2);

        return make_endless_cuboid(
            glm::vec3(side == Side::Left ? left_edge : right_edge - RAIL_THICKNESS, 1.0f - hrt, -0.5f),
            glm::vec3(RAIL_THICKNESS, 0.0f, 0.0f),
            glm::vec3(0.0f, rt_component, rt_component),
            glm::vec3(0.0f, -1.0f, 1.0f),
            RAIL_COLOR
        );
    }

    constexpr auto make_beam(unsigned int n) {
        float n_float = static_cast<float>(n);
        float total_length = gcem::sqrt(1.0f + 1.0f);
        unsigned int num_beams = 5;
        float beam_gap = TRACK_LENGTH / static_cast<float>(num_beams);
        float half_beam_gap = beam_gap / 2.0f;
        float right_inner_edge = (TRACK_WIDTH / 2.0f) - RAIL_THICKNESS;
        float left_inner_edge = -right_inner_edge;
        float beam_length = right_inner_edge - left_inner_edge;
        float hbt = BEAM_THICKNESS / 2.0f;

        return make_endless_cuboid(
            glm::vec3(right_inner_edge, half_beam_gap + (beam_gap * n_float), 0.5f - half_beam_gap - (beam_gap * n_float) - hbt),
            glm::vec3(0.0f, 0.0f, BEAM_THICKNESS),
            glm::vec3(0.0f, BEAM_THICKNESS, 0.0f),
            glm::vec3(-beam_length, 0.0f, 0.0f),
            BEAM_COLOR
        );
    }

    constexpr auto make_filler(Side side) {
        float hrt = RAIL_THICKNESS / 2.0f;
        float rt_component = RAIL_THICKNESS / gcem::sqrt(2);
        float side_x = side == Side::Left
                       ? (-TRACK_WIDTH / 2.0f)
                       : ((TRACK_WIDTH / 2.0f) - RAIL_THICKNESS);

        // Left filler
        auto left_face = make_triangle(
            glm::vec3(side_x, 1.0f - hrt, -0.5f),
            glm::vec3(side_x, 1.0f - hrt + rt_component, -0.5f + rt_component),
            glm::vec3(side_x, 1.0f + hrt, -0.5f),
            RAIL_COLOR
        );

        auto right_face = make_triangle(
            glm::vec3(side_x + RAIL_THICKNESS, 1.0f - hrt, -0.5f),
            glm::vec3(side_x + RAIL_THICKNESS, 1.0f + hrt, -0.5f),
            glm::vec3(side_x+ RAIL_THICKNESS, 1.0f - hrt + rt_component, -0.5f + rt_component),
            RAIL_COLOR
        );

        auto top_face = make_rect(
            glm::vec3(side_x, 1.0f - hrt + rt_component, -0.5f + rt_component),
            glm::vec3(RAIL_THICKNESS, 0.0f, 0.0f),
            glm::vec3(side_x, 1.0f + hrt, -0.5f) - 
            glm::vec3(side_x, 1.0f - hrt + rt_component, -0.5f + rt_component),
            RAIL_COLOR
        );

        return combine_all(left_face, right_face, top_face);
    }
}

constexpr auto SLOPED_TRACK = combine_all(
    sloped::make_rail(sloped::Side::Left),
    sloped::make_rail(sloped::Side::Right),

    sloped::make_beam(0),
    sloped::make_beam(1),
    sloped::make_beam(2),
    sloped::make_beam(3),
    sloped::make_beam(4),

    sloped::make_filler(sloped::Side::Left),
    sloped::make_filler(sloped::Side::Right)
);
