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

    minigl_obj *square_blue = minigl_obj_create_quad(myapp, 100.0f, 100.0f, 50.0f, blue, NULL, "mainshader", MINIGL_DYNAMIC);
    minigl_obj *body1 = minigl_obj_create_quad(myapp, 150.0f, 100.0f, 50.0f, blue, "body1", "mainshader", MINIGL_STATIC);
    minigl_obj *body2 = minigl_obj_create_quad(myapp, 250.0f, 300.0f, 50.0f, blue, "body1", "mainshader", MINIGL_STATIC);

    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object(scene, square_blue);
    minigl_scene_attach_object(scene, body1);
    minigl_scene_attach_object(scene, body2);
    minigl_engine_attach_scene(myapp, scene);



    double prev_time = glfwGetTime();
    int frame_count = 0;
    double fps_last_time = 0;

    //main loop
    while (!glfwWindowShouldClose(myapp->window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        update_input(&(myapp->engine_input_manager));
        glfwPollEvents();
        process_input(myapp, square_blue);

        double current_time = glfwGetTime();
        double delta_time = current_time - prev_time;


        minigl_process_movement(myapp, delta_time);
        minigl_draw(myapp);




        //put FPS in title bar
        frame_count++;
        if (current_time - fps_last_time >= 1.0) {
            int fps = frame_count;
            char title[256];
            snprintf(title, sizeof(title), "miniGL - [FPS: %d]", fps);
            glfwSetWindowTitle(myapp->window, title);
            frame_count = 0;
            fps_last_time = current_time;
        }


        glfwSwapBuffers(myapp->window);
    }


    glfwTerminate();
    return 0;
}



void process_input(minigl_engine *minigl_engine, minigl_obj *obj){
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(minigl_engine->window, GLFW_TRUE);
    }

    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_W)) {
        obj->velocity[0] = 0.0f;
        obj->velocity[1] = 1.0f;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_A)) {
        obj->velocity[0] = -1.0f;
        obj->velocity[1] = 0.0f;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_S)) {
        obj->velocity[0] = 0.0f;
        obj->velocity[1] = 0.0f;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_D)) {
        obj->velocity[0] = 1.0f;
        obj->velocity[1] = 0.0f;
    }
    
}
