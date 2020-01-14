#pragma once

// Central place for ALL global state

namespace globals {
    // Struct for the window size
    struct WindowDimensions {
        float aspect_ratio() const;

        int width;
        int height;
    };

    // Store a global one
    extern WindowDimensions WINDOW_DIMS;
    
    // Store whether the mouse was just clicked, so we know to handle the event
    extern bool mouse_just_clicked;
}
