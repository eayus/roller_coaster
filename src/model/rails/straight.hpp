#pragma once

#include <model/basic.hpp>
#include <model/transform.hpp>
#include <model/rails/config.hpp>


constexpr auto make_straight_rail() {
    float right_edge = TRACK_WIDTH / 2.0f;
    float left_edge = -right_edge;
    float front_edge = TRACK_LENGTH / 2.0f;
    float back_edge = -front_edge;
    float rail_top_edge = RAIL_THICKNESS / 2.0f;
    float rail_bot_edge = -rail_top_edge;
    float beam_top_edge = BEAM_THICKNESS / 2.0f;
    float beam_bot_edge = -beam_top_edge;
    float beam_length = TRACK_WIDTH - (2.0f * RAIL_THICKNESS);
    float beam_gap = TRACK_LENGTH / 4.0f;
    float initial_beam_offset = beam_gap / 2.0f;

    // Rails
    auto left_rail = make_endless_cuboid(
        glm::vec3(left_edge, rail_bot_edge, back_edge),
        glm::vec3(RAIL_THICKNESS, 0.0f, 0.0f),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        glm::vec3(0.0f, 0.0f, TRACK_LENGTH),
        RAIL_COLOR
    );

    auto right_rail = translate_model(
        left_rail,
        glm::vec3(TRACK_WIDTH - RAIL_THICKNESS, 0.0f, 0.0f)
    );


    // Beams
    auto first_beam = make_endless_cuboid(
        glm::vec3(
            right_edge - RAIL_THICKNESS,
            beam_bot_edge,
            back_edge + initial_beam_offset - (BEAM_THICKNESS / 2.0)
        ),
        glm::vec3(0.0f, 0.0f, BEAM_THICKNESS),
        glm::vec3(0.0f, BEAM_THICKNESS, 0.0f),
        glm::vec3(-beam_length, 0.0f, 0.0f),
        BEAM_COLOR
    );

    auto second_beam = translate_model(
        first_beam,
        glm::vec3(0.0f, 0.0f, beam_gap)
    );

    auto third_beam = translate_model(
        second_beam,
        glm::vec3(0.0f, 0.0f, beam_gap)
    );
    
    auto fourth_beam = translate_model(
        third_beam,
        glm::vec3(0.0f, 0.0f, beam_gap)
    );

    
    // Combine
    return combine_all(
        left_rail,
        right_rail,
        first_beam,
        second_beam,
        third_beam,
        fourth_beam
    );
}

constexpr auto STRAIGHT_TRACK = make_straight_rail();
