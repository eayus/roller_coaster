#pragma once


namespace globals {
    struct WindowDimensions {
        float aspect_ratio() const;

        int width;
        int height;
    };

    extern WindowDimensions WINDOW_DIMS;
}
