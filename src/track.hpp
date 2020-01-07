#pragma once

#include <model/tracks.hpp>
#include <util.hpp>

struct TrackSegment {
    TrackType type;
    glm::ivec3 position;
    Direction direction;
};

