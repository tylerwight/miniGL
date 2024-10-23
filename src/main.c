#include "minigl.h"

//todo
// implement audio
// great minigl object can contain audio

void process_input(minigl_engine *minigl_engine, minigl_obj *obj);

int main(){
    minigl_engine *myapp = minigl_init(1024.0f, 768.0f, "miniGL");

    //create some colors
    color red, green, blue;
    color_set(&red, 1.0f, 0.0f, 0.0f, 1.0f);
    color_set(&green, 0.0f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.0f, 0.0f, 1.0f, 1.0f);

    minigl_shader_load(myapp, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", "mainshader");
    minigl_texture_load(myapp, "assets/snek_head.png", "head");
    minigl_texture_load(myapp, "assets/snek_body1.png", "body1");

    minigl_obj *test = minigl_obj_create_quad(myapp, 100.0f, 100.0f, 50.0f, blue, NULL, "mainshader", MINIGL_DYNAMIC);
    minigl_obj *test2 = minigl_obj_create_quad(myapp, 150.0f, 100.0f, 50.0f, blue, "body1", "mainshader", MINIGL_STATIC);
    minigl_obj *test3 = minigl_obj_create_quad(myapp, 250.0f, 300.0f, 50.0f, blue, "body1", "mainshader", MINIGL_STATIC);

    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object(scene, test);
    minigl_scene_attach_object(scene, test2);
    minigl_scene_attach_object(scene, test3);
    minigl_engine_attach_scene(myapp, scene);



    double previousTime = glfwGetTime();
    int frameCount = 0;

    //main loop
    while (!glfwWindowShouldClose(myapp->window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        update_input(&(myapp->engine_input_manager));
        glfwPollEvents();
        process_input(myapp, test);
        minigl_process_movement(myapp);
        minigl_draw(myapp);


        double currentTime = glfwGetTime();
        frameCount++;
        if (currentTime - previousTime >= 1.0) {
            // Calculate FPS
            int fps = frameCount;
            char title[256];
            snprintf(title, sizeof(title), "miniGL - [FPS: %d]", fps);
            glfwSetWindowTitle(myapp->window, title);
            frameCount = 0;
            previousTime = currentTime;
        }


        glfwSwapBuffers(myapp->window);
    }


    //renderable_object_delete(&square_red);
    glfwTerminate();
    return 0;
}



void process_input(minigl_engine *minigl_engine, minigl_obj *obj){
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(minigl_engine->window, GLFW_TRUE);
    }

    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_W)) {
        minigl_obj_set_position(obj, 200.0f, 200.0f);
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_A)) {
        minigl_obj_set_position(obj, 100.0f, 100.0f);
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_S)) {
        obj->velocity[0] = 0.0f;
        obj->velocity[1] = 0.0f;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_D)) {
        obj->velocity[0] = 0.1f;
        obj->velocity[1] = 0.1f;
    }
    
}




/*
int main(){
    minigl_engine *myapp = minigl_init(1024.0f, 768.0f, "miniGL");

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

    //Create quad vertex, index data, vertex attributes
    quad *quad_red = quad_create(100.0f, 150.0f, 150.0f, red, texture_head);
    quad *quad_green = quad_create(400.0f, 150.0f, 100.0f, green, texture_head);
    quad *quad_blue = quad_create(700.0f, 150.0f, 50.0f, blue, NULL);
    quad *quad_test = quad_create(700.0f, 350.0f, 50.0f, blue, texture_body);

    // create renderable objects
    renderable_object square_red, square_green, square_blue, square_test;
    renderable_object_create_fromquad(&square_red, quad_red, main_shader);
    renderable_object_create_fromquad(&square_green, quad_green, main_shader);
    renderable_object_create_fromquad(&square_blue, quad_blue, main_shader);
    renderable_object_create_fromquad(&square_test, quad_test, main_shader);

    //Link some of them into a batch.
    renderable_batch *batch1 = calloc(1, sizeof(renderable_batch));
    renderable_batch_attach_object(batch1, &square_red);
    renderable_batch_attach_object(batch1, &square_green);
    renderable_batch_attach_object(batch1, &square_blue);
    renderable_batch_initialize(batch1);    
    renderable_batch_update_data(batch1);

    //create the main renderer and link the batch and 1 object
    renderer *main_renderer = calloc(1, sizeof(renderer));
    main_renderer->cam = camera_create(1024.0f, 768.0f);
    main_renderer->current_shader = NULL;
    renderer_attach_batch(main_renderer, batch1);
    renderer_attach_object(main_renderer, &square_test);
    
    
    
    //nuklear setup
    struct nuklear_container debug_menu;
    struct nuklear_debug_menu debug_menu_data;
    nuklear_container_setup(myapp->window, &debug_menu);
    nuklear_debug_menu_setup(&debug_menu_data, "Debug Menu", 200.0f, 200.0f, 400.0f, 200.0f);

    initialize_input(myapp->window);
    glfwSetWindowUserPointer(myapp->window, myapp);

    double previousTime = glfwGetTime();
    int frameCount = 0;

    //main loop
    while (!glfwWindowShouldClose(myapp->window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        update_input(&(myapp->engine_input_manager));
        glfwPollEvents();
        process_input(myapp, &square_test);


        nuklear_debug_menu_draw(myapp->window, &debug_menu, &debug_menu_data);
        renderer_draw(main_renderer);


        double currentTime = glfwGetTime();
        frameCount++;
        if (currentTime - previousTime >= 1.0) {
            // Calculate FPS
            int fps = frameCount;
            char title[256];
            snprintf(title, sizeof(title), "miniGL - [FPS: %d]", fps);
            glfwSetWindowTitle(myapp->window, title);
            frameCount = 0;
            previousTime = currentTime;
        }


        glfwSwapBuffers(myapp->window);
    }


    renderable_object_delete(&square_red);
    glfwTerminate();
    return 0;
}

*/