#include <GL/glew.h>
#include <application.hpp>
#include <shader.hpp>
#include <shader_program.hpp>
#include <string>
#include <iostream>
#include <cart_camera.hpp>
#include <roller_coaster.hpp>
#include <cart.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <globals.hpp>
#include <free_camera.hpp>
#include <ground.hpp>
#include <screen_texture_renderer.hpp>
#include <virtual_screen.hpp>
#include <ui/font.hpp>
#include <ui/text.hpp>
#include <ui/button.hpp>

constexpr glm::vec3 SKY_BLUE = glm::vec3(0.53f, 0.81f, 0.92f);

// Handling of mouse input
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Set global variable so we can access the mouse state in the rest of our code.
        globals::mouse_just_clicked = true;
    }
}

enum class GameMode {
    Observer,         // Move camera around freely
    Ride,             // Ride the roller coaster
};

Application::Application() {
    // Init Window
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    constexpr int DEFAULT_WIN_WIDTH = 1440;
    constexpr int DEFAULT_WIN_HEIGHT = 810;

    this->window = glfwCreateWindow(
        DEFAULT_WIN_WIDTH,
        DEFAULT_WIN_HEIGHT,
        "Hello World",
        nullptr,
        nullptr
    );

    globals::WINDOW_DIMS.width = DEFAULT_WIN_WIDTH;
    globals::WINDOW_DIMS.height = DEFAULT_WIN_HEIGHT;

    // Setup opengl context and set callbacks
    glfwMakeContextCurrent(this->window);
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Init GLEW for access to modern OpenGL
    glewExperimental = true;
    glewInit();
}

// Cleanup afterwards
Application::~Application() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Application::main_loop() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // For use in transparency


    // Location of light source in scene
    glm::vec3 light_pos(30.0, 90.0, 17.5);


    // Create all shaders we are using, and set default uniforms.
    ShaderProgram colored_mesh_shader( // Most common shader used, draws a basic mesh with color info
        VertexShader::from_filepath("res/shaders/colored_mesh.vert"),
        FragmentShader::from_filepath("res/shaders/colored_mesh.frag")
    );
    colored_mesh_shader.bind();
    colored_mesh_shader.set_uniform_vec3("light_position", light_pos);

    ShaderProgram screen_texture_shader( // Draws a texture to whole screen
        VertexShader::from_filepath("res/shaders/screen_texture.vert"),
        FragmentShader::from_filepath("res/shaders/screen_texture.frag")
    );

    ShaderProgram text_shader( // Draws text using textures
        VertexShader::from_filepath("res/shaders/text.vert"),
        FragmentShader::from_filepath("res/shaders/text.frag")
    );
    text_shader.bind();
    text_shader.set_uniform_float("window_width", static_cast<float>(globals::WINDOW_DIMS.width));
    text_shader.set_uniform_float("window_height", static_cast<float>(globals::WINDOW_DIMS.height));

    ShaderProgram ui_shader( // Draws buttons
        VertexShader::from_filepath("res/shaders/ui.vert"),
        FragmentShader::from_filepath("res/shaders/ui.frag")
    );
    ui_shader.bind();
    ui_shader.set_uniform_float("window_width", static_cast<float>(globals::WINDOW_DIMS.width));
    ui_shader.set_uniform_float("window_height", static_cast<float>(globals::WINDOW_DIMS.height));


    // Create main componenets of the scene
    RollerCoaster main_coaster("res/roller_coasters/test.track");

    Ground grnd = Ground::underneath_coaster(main_coaster);

    std::vector<Cart> carts;
    carts.push_back(main_coaster.add_cart());
    carts.push_back(Cart::behind(carts[0]));
    carts.push_back(Cart::behind(carts[1]));


    // Create cameras
    CartCamera cart_camera(carts.at(0));
    FreeCamera free_camera(glm::vec3(0, 0, 2), *this->window);

    
    // Game state vvariables
    GameMode gm = GameMode::Observer;
    bool paused = false;

    // When we pause, set these coordinates to mouse pos, and restore them when unpausing
    double cursor_x_paused;
    double cursor_y_paused; 

    // Class for rendering a texture to the entire screen
    ScreenTextureRenderer st_renderer;

    // Two "VirtualScreen"s, which are wrappers around a framebuffer and texture. Create
    // two here so we can ping pong between them when running blur effect.
    std::array<VirtualScreen, 2> vs;


    // UI: buttons and fonts
    Font fira_mono("res/fonts/fira_mono_regular.png");
    
    float screen_center_x = static_cast<float>(globals::WINDOW_DIMS.width) * 0.5f;
    Button resume_button(
        glm::vec2(screen_center_x, 500.0f),
        glm::vec2(380.0f, 80.0f)
    );
    Text resume_text(
        "resume game",
        fira_mono,
        glm::vec2(screen_center_x - 165.0f, 500.0f - 16.0f)
    );

    Button quit_button(
        glm::vec2(screen_center_x, 300.0f),
        glm::vec2(380.0f, 80.0f)
    );
    Text quit_text(
        "quit game",
        fira_mono,
        glm::vec2(screen_center_x - 140.0f, 300.0f - 16.0f)
    );

    // Background should be sky blue.
    glClearColor(SKY_BLUE.x, SKY_BLUE.y, SKY_BLUE.z, 1.0f);

    while (!glfwWindowShouldClose(this->window)) {
        // Depending on what mode we're in, set the uniforms using the appropriate camera
        if (gm == GameMode::Ride) {
            // Cart Camera when we're riding the coaster
            colored_mesh_shader.bind();
            colored_mesh_shader.set_uniform_mat4("camera_matrix", cart_camera.calculate_matrix());
            colored_mesh_shader.set_uniform_vec3("camera_position", cart_camera.get_position());
        } else if (gm == GameMode::Observer) {
            // Free Camera when we're "observing"
            colored_mesh_shader.bind();
            colored_mesh_shader.set_uniform_mat4("camera_matrix", free_camera.calculate_matrix());
            colored_mesh_shader.set_uniform_vec3("camera_position", free_camera.get_position());
        }


        // Update the first cart, then update the later carts based on the one in front
        carts.at(0).update();
        for (int i = 1; i < carts.size(); i++) {
            carts.at(i).update_behind(carts.at(i - 1));
        }

        // When riding the roller coaster, we're interested in the front cart's position
        auto cart_pos = carts.front().calc_position();

    
        // Lambda to easily render the entire scene. We may have to do this process several
        // times depending on post processing effects.
        auto render_scene = [&](ShaderProgram& shader) {
            shader.bind();
            shader.set_uniform_mat4("model_matrix", glm::mat4(1.0f));
            main_coaster.draw();
            grnd.draw();
            for (auto& cart : carts) cart.draw(shader);
        };

        // Render the scene once to one of our virtual screens (framebuffers)
        size_t vs_target = 0;
        vs[vs_target].bind_as_target();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            render_scene(colored_mesh_shader);

        vs_target = 1;

        // Guassian Blur effect:
        // Begin loop which ping pongs between the framebuffers, bluring a little each time
        
        int blur_iterations = paused ? 32 : 0;         // 32 Iterations. Unless we're not paused, in which case don't blur at all
        for (int i = 0; i < blur_iterations; i++) {
            size_t vs_source = (vs_target + 1) % 2;
            vs[vs_source].bind_as_source();
            vs[vs_target].bind_as_target();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Blur blur_type = i % 2 == 0 ? Blur::Horizontal : Blur::Vertical;
            st_renderer.draw_texture(blur_type);

            // Swap target and source now
            vs_target = vs_source;
        }

        // Now we've blurred the texture, render it to the window.
        VirtualScreen::bind_none_as_target();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            size_t vs_source = (vs_target + 1) % 2;
            vs[vs_source].bind_as_source();
            st_renderer.draw_texture(Blur::None);
            // Render pause menu stuff
            if (paused) {
                ui_shader.bind();
                resume_button.draw();
                quit_button.draw();
                text_shader.bind();
                quit_text.draw();
                resume_text.draw();
            }


        // Swap buffers, blitting our reuslt to the screen
        glfwSwapBuffers(this->window);
        glfwPollEvents();

        // Handle inputs.
        
        if (!paused && glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            // ESC is pause key. We record our mouse position so when the user unpauses
            // we can restore their viewing angle.
            paused = true;
            glfwGetCursorPos(this->window, &cursor_x_paused, &cursor_y_paused);
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        
        // Set free camera
        if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS && !paused) gm = GameMode::Observer;
        
        // Set ride camera
        if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS && !paused) gm = GameMode::Ride;

        // If we're in free camera, we need to handle the inputs to move it around
        if (gm == GameMode::Observer && !paused) free_camera.handle_inputs(*this->window);

        // Handle our button inputs
        if (paused && globals::mouse_just_clicked) {
            double cursor_x;
            double cursor_y;
            glfwGetCursorPos(this->window, &cursor_x, &cursor_y);
            cursor_x = globals::WINDOW_DIMS.width - cursor_x;
            cursor_y = globals::WINDOW_DIMS.height - cursor_y;

            // Is the click in the rect of any button?
            if (quit_button.is_in_bounds(glm::vec2(cursor_x, cursor_y))) break;
            if (resume_button.is_in_bounds(glm::vec2(cursor_x, cursor_y))) {
                paused = false;
                glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetCursorPos(this->window, cursor_x_paused, cursor_y_paused);
            }

            // We've processed the event, so reset the variable
            globals::mouse_just_clicked = false;
        }
    }
}
