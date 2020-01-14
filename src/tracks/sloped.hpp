#pragma once

#include <model/rails/slope.hpp>
#include <model/transform.hpp>
#include <glm/glm.hpp>

#include <model/rails/path.hpp>

namespace tracks::sloped {
    namespace down {
        constexpr auto MODEL = translate_model(rotate_180(SLOPED_TRACK), glm::vec3(0, -1, 0));

        constexpr glm::vec3 calc_position(float percent) {
            return glm::vec3(0.0f, -percent, 0.5 - percent);
        }

        constexpr Rotation calc_rotation(float percent, TrackType prev_type, TrackType next_type) {
            if (percent <= 0.2f && prev_type != TrackType::SlopeDown) {
                return Rotation(0.0f, glm::radians(-45.0f) * percent / 0.2f);
            }

            if (percent >= 0.8f && next_type != TrackType::SlopeDown) {
                return Rotation(0.0f, glm::radians(-45.0f) * (1.0f - percent) / 0.2f);
            }

            return Rotation(0.0f, glm::radians(-45.0f));
        }
    }
}
