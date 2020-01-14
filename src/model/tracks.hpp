#pragma once

#include <model/model.hpp>
#include <model/rails/path.hpp>
#include <boost/optional.hpp>
#include <utility>
#include <util.hpp>

#include <model/rails/straight.hpp>
#include <model/rails/curved.hpp>
#include <model/rails/slope.hpp>

#include <tracks/sloped.hpp>
#include <tracks/curved.hpp>

#include <model/track_type.hpp>

using PositionFunc = glm::vec3 (*)(float);
using RotationFunc = Rotation (*)(float, TrackType, TrackType);

struct TrackData {
    constexpr TrackData(ModelRef model, PositionFunc calc_position, RotationFunc calc_rotation, float length, std::pair<glm::ivec3, Direction> relative_finish)
        : model(model)
        , calc_position(calc_position)
        , calc_rotation(calc_rotation)
        , length(length)
        , relative_finish(relative_finish) {}

    ModelRef model;
    PositionFunc calc_position;
    RotationFunc calc_rotation;
    float length;
    std::pair<glm::ivec3, Direction> relative_finish; // pos, dir
};

namespace tracks {


    constexpr TrackData FORWARD_DATA = TrackData(
        ModelRef(STRAIGHT_TRACK),
        &straight_track_position,
        &straight_track_rotation,
        TRACK_FORWARD_LEN,
        std::make_pair(glm::ivec3(0, 0, -1), Direction::Up)
    );

    constexpr TrackData CURVE_LEFT_DATA = TrackData(
        ModelRef(CURVED_TRACK),
        &curved_track_position,
        &curved_track_rotation,
        TRACK_CURVE_LEN,
        std::make_pair(glm::ivec3(-1, 0, 0), Direction::Left)
    );

    constexpr TrackData CURVE_RIGHT_DATA = TrackData(
        ModelRef(tracks::curved::left::MODEL),
        &tracks::curved::left::calc_position,
        &tracks::curved::left::calc_rotation,
        TRACK_CURVE_LEN,
        std::make_pair(glm::ivec3(1, 0, 0), Direction::Right)
    );

    constexpr TrackData SLOPE_UP_DATA = TrackData(
        ModelRef(SLOPED_TRACK),
        &sloped_track_position,
        &sloped_track_rotation,
        TRACK_SLOPED_LEN,
        std::make_pair(glm::ivec3(0, 1, -1), Direction::Up)
    );

    constexpr TrackData SLOPE_DOWN_DATA = TrackData(
        ModelRef(tracks::sloped::down::MODEL),
        &tracks::sloped::down::calc_position,
        &tracks::sloped::down::calc_rotation,
        TRACK_SLOPED_LEN,
        std::make_pair(glm::ivec3(0, -1, -1), Direction::Up)
    );

    inline boost::optional<TrackType> parse(const std::string& name) {
        if (name == "forward") return TrackType::Forward;
        if (name == "curve_left") return TrackType::CurveLeft;
        if (name == "curve_right") return TrackType::CurveRight;
        if (name == "slope_up") return TrackType::SlopeUp;
        if (name == "slope_down") return TrackType::SlopeDown;

        return boost::none;
    }

    constexpr const TrackData& data_from_type(TrackType type) {
        switch (type) {
            case TrackType::Forward: return FORWARD_DATA;
            case TrackType::CurveLeft: return CURVE_LEFT_DATA;
            case TrackType::SlopeUp: return SLOPE_UP_DATA;
            case TrackType::SlopeDown: return SLOPE_DOWN_DATA;
            case TrackType::CurveRight: return CURVE_RIGHT_DATA;
        }
    }

    constexpr ModelRef model(TrackType type) {
        auto data = data_from_type(type);
        return data.model;
    }

    constexpr PositionFunc calc_position(TrackType type) {
        auto data = data_from_type(type);
        return data.calc_position;
    }

    constexpr RotationFunc calc_rotation(TrackType type) {
        auto data = data_from_type(type);
        return data.calc_rotation;
    }

    constexpr float length(TrackType type) {
        auto data = data_from_type(type);
        return data.length;
    }

    constexpr std::pair<glm::ivec3, Direction> relative_finish(TrackType type) {
        auto data = data_from_type(type);
        return data.relative_finish;
    }
}
