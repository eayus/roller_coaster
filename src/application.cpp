#include <GL/glew.h>
#include <application.hpp>
#include <shader.hpp>
#include <shader_program.hpp>
#include <string>
#include <iostream>
#include <camera.hpp>
#include <cart_camera.hpp>
#include <model/scene.hpp>
#include <roller_coaster.hpp>
#include <cart.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <globals.hpp>
#include <free_camera.hpp>

void gl_error_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    void* user_param)
{
    std::cerr << "[OPENGL MESSAGE] " << message << std::endl << std::endl;
}

enum class GameMode {
    Observer,
    Ride,
};

Application::Application() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);

    std::cout << video_mode->width << ", " << video_mode->height << std::endl;

    this->window = glfwCreateWindow(
        video_mode->width,
        video_mode->height,
        "Hello World",
        monitor,
        nullptr
    );

    globals::WINDOW_DIMS.width = video_mode->width;
    globals::WINDOW_DIMS.height = video_mode->height;

    glfwMakeContextCurrent(this->window);
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = true;
    glewInit();
    glDebugMessageCallback(gl_error_callback, nullptr);
}

Application::~Application() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Application::main_loop() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    ShaderProgram colored_mesh_shader(
        VertexShader::from_filepath("res/shaders/colored_mesh.vert"),
        FragmentShader::from_filepath("res/shaders/colored_mesh.frag")
    );

    RollerCoaster main_coaster("res/roller_coasters/test.track");

    std::vector<Cart> carts;
    carts.push_back(main_coaster.add_cart());
    carts.push_back(Cart::behind(carts[0]));
    carts.push_back(Cart::behind(carts[1]));


    CartCamera cart_camera(carts.back());
    FreeCamera free_camera(glm::vec3(0, 0, 2), *this->window);


    GameMode gm = GameMode::Observer;

    while (!glfwWindowShouldClose(this->window)) {
        colored_mesh_shader.bind();

        if (gm == GameMode::Ride) {
            colored_mesh_shader.set_uniform_mat4("camera_matrix", cart_camera.calculate_matrix());
            colored_mesh_shader.set_uniform_vec3("camera_position", cart_camera.get_position());
        } else if (gm == GameMode::Observer) {
            colored_mesh_shader.set_uniform_mat4("camera_matrix", free_camera.calculate_matrix());
            colored_mesh_shader.set_uniform_vec3("camera_position", free_camera.get_position());
        }


        for (auto& cart : carts) cart.update();
        auto cart_pos = carts[0].calc_position();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // DRAWING
        colored_mesh_shader.bind();

        // Draw Coaster
        colored_mesh_shader.set_uniform_mat4("model_matrix", glm::mat4(1.0f));
        main_coaster.draw();

        // Draw Cart
        for (auto& cart : carts) cart.draw(colored_mesh_shader);


        glfwSwapBuffers(this->window);

        glfwPollEvents();

        if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
        if (glfwGetKey(this->window, GLFW_KEY_1) == GLFW_PRESS) glEnable(GL_CULL_FACE);
        if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS) gm = GameMode::Observer;
        if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS) gm = GameMode::Ride;

        if (gm == GameMode::Observer) free_camera.handle_inputs(*this->window);
    }
}
