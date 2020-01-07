#pragma once

#include <glm/glm.hpp>
#include <model/transform.hpp>
#include <model/rails/curved.hpp>

#include <model/rails/path.hpp>

namespace tracks::curved {
    namespace left {
        constexpr auto MODEL = rotate_270(CURVED_TRACK);

        inline glm::vec3 calc_position(float percent) {
            const float theta = percent * glm::radians(90.0f);

            return glm::vec3(
                0.5f - (0.5f * glm::cos(theta)),
                0.0f,
                0.5f - (0.5f * glm::sin(theta))
            );
        }

        constexpr Rotation calc_rotation(float percent) {
            return Rotation(
                percent * glm::radians(90.0f),
                0.0f
            );
        }
    }
}
