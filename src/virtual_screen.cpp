#include <virtual_screen.hpp>
#include <globals.hpp>
#include <cstdlib>
#include <iostream>

VirtualScreen::VirtualScreen() {
    glGenFramebuffers(1, &this->frame_buffer);
    glGenTextures(1, &this->texture);
    glGenRenderbuffers(1, &this->render_buffer);

    // Make Texture
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        globals::WINDOW_DIMS.width,
        globals::WINDOW_DIMS.height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Make renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, this->render_buffer);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        globals::WINDOW_DIMS.width,
        globals::WINDOW_DIMS.height
    );


    // Attach them
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        this->texture,
        0
    );

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        this->render_buffer
    );

    // Check for errors
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
	    std::cerr << "[ERROR]: framebuffer not complete" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}


VirtualScreen::~VirtualScreen() {
    glDeleteRenderbuffers(1, &this->render_buffer);
    glDeleteTextures(1, &this->texture);
    glDeleteFramebuffers(1, &this->frame_buffer);
}


void VirtualScreen::bind_as_target() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
}


void VirtualScreen::bind_as_source() {
    glBindTexture(GL_TEXTURE_2D, this->texture);
}


void VirtualScreen::bind_none_as_target() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
