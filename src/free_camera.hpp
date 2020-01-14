#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// Mouse sensitivity of the camera
constexpr float FREE_CAMERA_SENSITIVITY = 0.3f;

// Class representing a freely movable camera, controlled through mouse and keyboard

class FreeCamera {
public:
	FreeCamera(const glm::vec3& initial_position, GLFWwindow& window);
	glm::mat4 calculate_matrix() const;                                  // Calculate proj * view matrix
	glm::vec3 get_position() const;                                      // handle the mouse/keybaord inputs
    void handle_inputs(GLFWwindow& window);

private:
	glm::vec3 position; 
    // Use Euler angles to store our rotation
	float yaw;
    float pitch;

    // Store the last position of the cursor, so we can figure how much the user has moved the mouse
    double last_cursor_x;
    double last_cursor_y;
};
