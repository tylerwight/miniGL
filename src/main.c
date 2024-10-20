#include "minigl.h"


//todo
// implement audio
// change renderer to "batch" and only have one renderer that contains batches
// great minigl object can contain audio
//camera for view projection

void process_input(engine *engine, renderable_object *object);

int main(){
    engine test_app = {0};
    GLFWwindow* window;
    window = setup_opengl(1024,768);
    test_app.window = window;
    
    //setup shaders
    shader *main_shader = initialize_shader(1024.0f, 768.0f, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    //create some colors
    color red, green, blue;
    color_set(&red, 1.0f, 0.0f, 0.0f, 1.0f);
    color_set(&green, 0.0f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.0f, 0.0f, 1.0f, 1.0f);
    

    //load textures
    texture *texture_head = texture_load("assets/snek_head.png");
    texture *texture_body = texture_load("assets/snek_body1.png");

    //Create quad vertex + index data
    quad *quad_red = quad_create(100.0f, 150.0f, 150.0f, red, NULL);
    quad *quad_green = quad_create(400.0f, 150.0f, 100.0f, green, texture_head);
    quad *quad_blue = quad_create(700.0f, 150.0f, 50.0f, blue, NULL);
    quad *quad_test = quad_create(700.0f, 350.0f, 50.0f, blue, texture_body);

    // create some objects
    renderable_object square_red, square_green, square_blue, square_test;
    renderable_object_create_fromquad(&square_red, quad_red, main_shader);
    renderable_object_create_fromquad(&square_green, quad_green, main_shader);
    renderable_object_create_fromquad(&square_blue, quad_blue, main_shader);
    renderable_object_create_fromquad(&square_test, quad_test, main_shader);

    //link them to a renderer
    renderer *game_renderer = calloc(1, sizeof(renderer));
    renderer_attach_object(game_renderer, &square_red);
    renderer_attach_object(game_renderer, &square_green);
    renderer_attach_object(game_renderer, &square_blue);
    renderer_initialize(game_renderer);    
    renderer_update_data(game_renderer);
    
    
    //nuklear setup
    struct nuklear_container debug_menu;
    struct nuklear_debug_menu debug_menu_data;
    nuklear_container_setup(window, &debug_menu);
    nuklear_debug_menu_setup(&debug_menu_data, "Debug Menu", 200.0f, 200.0f, 400.0f, 200.0f);

    initialize_input(window);
    glfwSetWindowUserPointer(window, &test_app);

    
    double previousTime = glfwGetTime();
    int frameCount = 0;
    glfwSwapInterval(1);
    //main loop
    while (!glfwWindowShouldClose(window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        update_input(&(test_app.engine_input_manager));
        glfwPollEvents();
        process_input(&test_app, &square_test);

        nuklear_debug_menu_draw(window, &debug_menu, &debug_menu_data);
        //renderer_translate(game_renderer, 0.0f, 0.5f);
        
        renderable_object_draw(&square_test);
        renderer_draw(game_renderer);
        


        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - previousTime >= 1.0) {
            // Calculate FPS
            int fps = frameCount;
            char title[256];
            snprintf(title, sizeof(title), "miniGL - [FPS: %d]", fps);
            glfwSetWindowTitle(window, title);
            frameCount = 0;
            previousTime = currentTime;
        }

        glfwSwapBuffers(window);
        
    }


    renderable_object_delete(&square_red);
    glfwTerminate();
    return 0;
}



void process_input(engine *engine, renderable_object *object){
    if (is_key_down(&engine->engine_input_manager, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(engine->window, GLFW_TRUE);
    }

    if (is_key_down(&engine->engine_input_manager, GLFW_KEY_W)) {
        renderable_object_translate(object, 0.0f, 0.5f);
    }
    if (is_key_down(&engine->engine_input_manager, GLFW_KEY_A)) {
        renderable_object_translate(object, -0.5f, 0.0f);
    }
    if (is_key_down(&engine->engine_input_manager, GLFW_KEY_S)) {
        renderable_object_translate(object, 0.0f, -0.5f);
    }
    if (is_key_down(&engine->engine_input_manager, GLFW_KEY_D)) {
        renderable_object_translate(object, 0.5f, 0.0f);
    }
    
}