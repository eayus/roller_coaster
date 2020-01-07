#pragma once

#include <glm/glm.hpp>
#include <gcem/gcem.hpp>
#include <model/basic.hpp>
#include <model/transform.hpp>
#include <model/rails/config.hpp>
#include <std_extras/array.hpp>

using namespace std_extras;

// TODO: face culling fails on curved rail bottom

enum class Facing { In, Out };
constexpr Array<float, 7> make_curve_sequence(float flat_radius) {
    float segment_radius = 90.0f / 5.0f;
    float curve_radius = flat_radius / gcem::cos(glm::radians(segment_radius / 2.0f));

    return {{
        0.0f - 0.5f,
        curve_radius * gcem::sin(glm::radians(segment_radius * 0.5f)) - 0.5f,
        curve_radius * gcem::sin(glm::radians(segment_radius * 1.5f)) - 0.5f,
        curve_radius * gcem::sin(glm::radians(segment_radius * 2.5f)) - 0.5f,
        curve_radius * gcem::sin(glm::radians(segment_radius * 3.5f)) - 0.5f,
        curve_radius * gcem::sin(glm::radians(segment_radius * 4.5f)) - 0.5f,
        flat_radius - 0.5f
    }};
}


constexpr auto make_rail_curve_out(float flat_radius) {
    auto curve_sequence = make_curve_sequence(flat_radius);
    
    auto segment1 = make_rect(
        glm::vec3(curve_sequence.elems[1], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[5]),
        glm::vec3(curve_sequence.elems[0] - curve_sequence.elems[1], 0.0f, curve_sequence.elems[5] - curve_sequence.elems[6]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment2 = make_rect(
        glm::vec3(curve_sequence.elems[2], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[4]),
        glm::vec3(curve_sequence.elems[1] - curve_sequence.elems[2], 0.0f, curve_sequence.elems[4] - curve_sequence.elems[5]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment3 = make_rect(
        glm::vec3(curve_sequence.elems[3], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[3]),
        glm::vec3(curve_sequence.elems[2] - curve_sequence.elems[3], 0.0f, curve_sequence.elems[3] - curve_sequence.elems[4]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment4 = make_rect(
        glm::vec3(curve_sequence.elems[4], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[2]),
        glm::vec3(curve_sequence.elems[3] - curve_sequence.elems[4], 0.0f, curve_sequence.elems[2] - curve_sequence.elems[3]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment5 = make_rect(
        glm::vec3(curve_sequence.elems[5], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[1]),
        glm::vec3(curve_sequence.elems[4] - curve_sequence.elems[5], 0.0f, curve_sequence.elems[1] - curve_sequence.elems[2]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment6 = make_rect(
        glm::vec3(curve_sequence.elems[6], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[0]),
        glm::vec3(curve_sequence.elems[5] - curve_sequence.elems[6], 0.0f, curve_sequence.elems[0] - curve_sequence.elems[1]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );

    return combine_all(segment1, segment2, segment3, segment4, segment5, segment6);
}

constexpr auto make_rail_curve_in(float flat_radius) {
    auto curve_sequence = make_curve_sequence(flat_radius);
    
    auto segment1 = make_rect(
        glm::vec3(curve_sequence.elems[0], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[6]),
        glm::vec3(curve_sequence.elems[1] - curve_sequence.elems[0], 0.0f, curve_sequence.elems[6] - curve_sequence.elems[5]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment2 = make_rect(
        glm::vec3(curve_sequence.elems[1], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[5]),
        glm::vec3(curve_sequence.elems[2] - curve_sequence.elems[1], 0.0f, curve_sequence.elems[5] - curve_sequence.elems[4]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment3 = make_rect(
        glm::vec3(curve_sequence.elems[2], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[4]),
        glm::vec3(curve_sequence.elems[3] - curve_sequence.elems[2], 0.0f, curve_sequence.elems[4] - curve_sequence.elems[3]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment4 = make_rect(
        glm::vec3(curve_sequence.elems[3], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[3]),
        glm::vec3(curve_sequence.elems[4] - curve_sequence.elems[3], 0.0f, curve_sequence.elems[3] - curve_sequence.elems[2]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment5 = make_rect(
        glm::vec3(curve_sequence.elems[4], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[2]),
        glm::vec3(curve_sequence.elems[5] - curve_sequence.elems[4], 0.0f, curve_sequence.elems[2] - curve_sequence.elems[1]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );
    auto segment6 = make_rect(
        glm::vec3(curve_sequence.elems[5], -RAIL_THICKNESS / 2.0f, -curve_sequence.elems[1]),
        glm::vec3(curve_sequence.elems[6] - curve_sequence.elems[5], 0.0f, curve_sequence.elems[1] - curve_sequence.elems[0]),
        glm::vec3(0.0f, RAIL_THICKNESS, 0.0f),
        RAIL_COLOR
    );

    return combine_all(segment1, segment2, segment3, segment4, segment5, segment6);
}

constexpr auto make_rail_curve(Facing facing, float flat_radius) {
    if (facing == Facing::Out) {
        return make_rail_curve_out(flat_radius);
    } else {
        return make_rail_curve_in(flat_radius);
    }
}


constexpr auto make_rail_curve_top_segment(size_t n, float inner_flat_radius, float outer_flat_radius, bool facing_up) {
    auto inner_seq = make_curve_sequence(inner_flat_radius);
    auto outer_seq = make_curve_sequence(outer_flat_radius);

    glm::vec3 normal(0, facing_up ? 1 : -1, 0);
    float y = (facing_up ? 1.0 : - 1.0) * RAIL_THICKNESS / 2.0f;

    glm::vec3 top_left(outer_seq.elems[0 + n], y, -outer_seq.elems[6 - n]);
    glm::vec3 top_right(outer_seq.elems[1 + n], y, -outer_seq.elems[5 - n]);
    glm::vec3 bottom_left(inner_seq.elems[0 + n], y, -inner_seq.elems[6 - n]);
    glm::vec3 bottom_right(inner_seq.elems[1 + n], y, -inner_seq.elems[5 - n]);

    return Model<4, 6>(
        {
            Vertex(bottom_left, normal, RAIL_COLOR),
            Vertex(bottom_right, normal, RAIL_COLOR),
            Vertex(top_left, normal, RAIL_COLOR),
            Vertex(top_right, normal, RAIL_COLOR),
        },
        { 0, 1, 2, 1, 3, 2 }
    );
}

constexpr auto make_rail_curve_top(float inner_flat_radius, float outer_flat_radius, bool facing_up) {
    return combine_all(
        make_rail_curve_top_segment(0, inner_flat_radius, outer_flat_radius, facing_up),
        make_rail_curve_top_segment(1, inner_flat_radius, outer_flat_radius, facing_up),
        make_rail_curve_top_segment(2, inner_flat_radius, outer_flat_radius, facing_up),
        make_rail_curve_top_segment(3, inner_flat_radius, outer_flat_radius, facing_up),
        make_rail_curve_top_segment(4, inner_flat_radius, outer_flat_radius, facing_up),
        make_rail_curve_top_segment(5, inner_flat_radius, outer_flat_radius, facing_up)
    );
}

constexpr auto make_curved_beam(size_t n) {
    // hbt = half beam thickness
    float hbt = BEAM_THICKNESS / 2.0f;
    float outer_gap = (TRACK_LENGTH - TRACK_WIDTH) / 2.0f;

    auto outer_seq = make_curve_sequence(1.0f - outer_gap - RAIL_THICKNESS);
    auto inner_seq = make_curve_sequence(outer_gap + RAIL_THICKNESS);

    // First Beam
    glm::vec3 outer_left(outer_seq.elems[1 + n], -hbt, -outer_seq.elems[5 - n]);
    glm::vec3 outer_right(outer_seq.elems[2 + n], -hbt, -outer_seq.elems[4 - n]);
    glm::vec3 outer_delta = outer_right - outer_left;

    glm::vec3 inner_left(inner_seq.elems[1 + n], -hbt, -inner_seq.elems[5 - n]);
    glm::vec3 inner_right(inner_seq.elems[2 + n], -hbt, -inner_seq.elems[4 - n]);
    glm::vec3 inner_delta = inner_right - inner_left;

    float outer_distance = magnitude(outer_delta);
    float inner_distance = magnitude(inner_delta);

    float outer_left_percent = ((outer_distance / 2.0f) - hbt) / outer_distance;
    float outer_right_percent = ((outer_distance / 2.0f) + hbt) / outer_distance;

    float inner_left_percent = ((inner_distance / 2.0f) - hbt) / inner_distance;

    glm::vec3 bottom_back_left = outer_left + (outer_left_percent * outer_delta);
    glm::vec3 bottom_back_right = outer_left + (outer_right_percent * outer_delta);
    glm::vec3 bottom_forward_left = inner_left + (inner_left_percent * inner_delta);

    return make_endless_cuboid(
        bottom_back_left,
        bottom_back_right - bottom_back_left,
        glm::vec3(0.0f, BEAM_THICKNESS, 0.0f),
        bottom_forward_left - bottom_back_left,
        BEAM_COLOR
    );
}


constexpr auto make_curved_track() {
    float outer_gap = (TRACK_LENGTH - TRACK_WIDTH) / 2.0f;

    auto outer_rail_outer_curve = make_rail_curve(Facing::Out, 1.0f - outer_gap);
    auto outer_rail_inner_curve = make_rail_curve(Facing::In, 1.0f - outer_gap - RAIL_THICKNESS);

    auto inner_rail_outer_curve = make_rail_curve(Facing::In, outer_gap);
    auto inner_rail_inner_curve = make_rail_curve(Facing::Out, outer_gap + RAIL_THICKNESS);

    return combine_all(
        outer_rail_outer_curve,
        outer_rail_inner_curve,
        inner_rail_outer_curve,
        inner_rail_inner_curve,
        make_curved_beam(0),
        make_curved_beam(1),
        make_curved_beam(2),
        make_curved_beam(3),
        make_rail_curve_top(1.0f - outer_gap - RAIL_THICKNESS, 1.0f - outer_gap, true),
        make_rail_curve_top(1.0f - outer_gap - RAIL_THICKNESS, 1.0f - outer_gap, false),
        make_rail_curve_top(outer_gap, outer_gap + RAIL_THICKNESS, true),
        make_rail_curve_top(outer_gap, outer_gap + RAIL_THICKNESS, false)
    );
}


constexpr auto CURVED_TRACK = make_curved_track();
