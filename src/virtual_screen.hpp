#pragma once

#include <GL/glew.h>

// Class wrapping a framebuffer for easy access and drawing to.

class VirtualScreen {
public:
    VirtualScreen();
    ~VirtualScreen();
    void bind_as_target();
    void bind_as_source();

    // Bind no framebuffer, effectively making subsequence draw calls draw to the screen
    static void bind_none_as_target(); 

private:
    GLuint frame_buffer;
    GLuint texture;
    GLuint render_buffer;
};
