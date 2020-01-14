#include <free_camera.hpp>
#include <globals.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

FreeCamera::FreeCamera(const glm::vec3& initial_position, GLFWwindow& window)
    : position(initial_position)
    , yaw(0.0f)
    , pitch(0.0f)
{
    // Get the cursor position so we can calculate its movement relative later.
    glfwGetCursorPos(&window, &this->last_cursor_x, &this->last_cursor_y);
}


glm::mat4 FreeCamera::calculate_matrix() const {
    // Rotation component
    glm::mat3 rot_matrix = glm::mat3(glm::rotate(this->yaw, glm::vec3(0, -1, 0))
                                   * glm::rotate(this->pitch, glm::vec3(1, 0, 0)));


    // Project matrix
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        globals::WINDOW_DIMS.aspect_ratio(),
        0.1f,
        1000.0f
    );

    // View matrix
    glm::mat4 view = glm::lookAt(
        this->position,
        this->position + (rot_matrix * glm::vec3(0, 0, -1)),
        glm::vec3(0, 1, 0)
    );

    return proj * view;
}


glm::vec3 FreeCamera::get_position() const {
    return this->position;
}


void FreeCamera::handle_inputs(GLFWwindow& window) {
    // Handle mouse
    double new_cursor_x, new_cursor_y;
    glfwGetCursorPos(&window, &new_cursor_x, &new_cursor_y);
    double delta_cursor_x = new_cursor_x - this->last_cursor_x;
    double delta_cursor_y = new_cursor_y - this->last_cursor_y;
    this->last_cursor_x = new_cursor_x;
    this->last_cursor_y = new_cursor_y;

    // Update yaw/pitch
    this->yaw += glm::radians(static_cast<float>(delta_cursor_x) * FREE_CAMERA_SENSITIVITY);
    this->pitch += glm::radians(static_cast<float>(-delta_cursor_y) * FREE_CAMERA_SENSITIVITY);

    this->pitch = glm::clamp(this->pitch, glm::radians(-89.9f), glm::radians(89.9f));


    // Move vertically
    if (glfwGetKey(&window, GLFW_KEY_SPACE) == GLFW_PRESS) this->position.y += 0.1f;
    if (glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) this->position.y -= 0.1f;

    const auto yaw_matrix = glm::mat3(glm::rotate(this->yaw, glm::vec3(0, -1, 0)));
    const glm::vec3 facing = yaw_matrix * glm::vec3(0, 0, -1);
    const glm::vec3 right = glm::cross(facing, glm::vec3(0, 1, 0));

    // Horizontal movement
    if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS) this->position += facing * 0.1f;
    if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS) this->position -= facing * 0.1f;
    if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS) this->position += right * 0.1f;
    if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS) this->position -= right * 0.1f;
}
