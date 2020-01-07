#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class FreeCamera {
public:
	FreeCamera(const glm::vec3& initial_position, GLFWwindow& window);
	glm::mat4 calculate_matrix() const;
	glm::vec3 get_position() const;
    void handle_inputs(GLFWwindow& window);

private:
	glm::vec3 position;
	float yaw;
    float pitch;

    double last_cursor_x;
    double last_cursor_y;
};
