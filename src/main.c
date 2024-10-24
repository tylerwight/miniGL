#include "minigl.h"


void process_input(minigl_engine *minigl_engine, minigl_obj *obj, double delta_time);
void load_assets(minigl_engine *engine);
minigl_scene *create_scene1(minigl_engine *engine);
void check_collision(minigl_engine *engine, minigl_obj *obj);



int main(){
    minigl_engine *myapp = minigl_init(1024.0f, 768.0f, "miniGL");
    load_assets(myapp);

    minigl_scene *scene = create_scene1(myapp); 
    minigl_engine_attach_scene(myapp, scene);

    audio_source *test = audio_manager_create_source(&myapp->audio_manager, "test");
    audio_manager_play_source(test);

    double current_time = glfwGetTime();
    double prev_time = current_time;
    int frame_count = 0;
    double deltaTime = 0.0;

    double fps_last_time = 0;

    //main loop
    while (!glfwWindowShouldClose(myapp->window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        update_input(&(myapp->engine_input_manager));
        glfwPollEvents();
        
        double current_time = glfwGetTime();
        double delta_time = current_time - prev_time;
        prev_time = current_time;

        process_input(myapp, scene->objects[0], delta_time);
        check_collision(myapp, scene->objects[0]);

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



void process_input(minigl_engine *minigl_engine, minigl_obj *obj, double delta_time){
    int x = 0;
    int y = 1;
    float friction = 500.0f * delta_time;
    float speed = 100.0f;
    float gravity = -100.0f;

    printf("speed: %f\nfriction : %f\ngravity: %f\n", speed, friction, gravity);

    obj->velocity[y] += gravity;

    if (obj->velocity[x] > 0.0f){
        obj->velocity[x] = obj->velocity[x] - friction;
    }
    if (obj->velocity[x] < 0.0f){
        obj->velocity[x] = obj->velocity[x] + friction;
    }
    if (obj->velocity[y] > 0.0f){
        obj->velocity[y] = obj->velocity[y] - friction;
    }
    if (obj->velocity[y] < 0.0f){
        obj->velocity[y] = obj->velocity[y] + friction;
    }

    if (obj->velocity[x] > -0.1f && obj->velocity[x] < 1.0f){ obj->velocity[x] = 0.0f;}
    if (obj->velocity[y] > -0.1f && obj->velocity[y] < 1.0f){ obj->velocity[y] = 0.0f;}
    


    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(minigl_engine->window, GLFW_TRUE);
    }

    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_W)) {
        obj->velocity[y] = speed;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_A)) {
        obj->velocity[x] = -speed;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_S)) {
        obj->velocity[y] = -speed;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_D)) {
        obj->velocity[x] = speed;
    }
    
}


minigl_scene *create_scene1(minigl_engine *engine){
    //create some colors
    color red, green, blue;
    color_set(&red, 1.0f, 0.0f, 0.0f, 1.0f);
    color_set(&green, 0.0f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.0f, 0.0f, 1.0f, 1.0f);


    minigl_obj *objects[3];
    objects[0] = minigl_obj_create_quad(engine, 500.0f, 350.0f, 50.0f, blue, NULL, "mainshader", MINIGL_DYNAMIC);
    objects[1] = minigl_obj_create_quad(engine, 500.0f, 200.0f, 50.0f, blue, "body1", "mainshader", MINIGL_STATIC);
    objects[2] = minigl_obj_create_quad(engine, 750.0f, 300.0f, 50.0f, blue, "body1", "mainshader", MINIGL_STATIC);

    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object_many(scene, objects, 3);

    return scene;
}

void load_assets(minigl_engine *engine){
    minigl_shader_load(engine, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", "mainshader");
    minigl_texture_load(engine, "assets/snek_head.png", "head");
    minigl_texture_load(engine, "assets/snek_body1.png", "body1");
    audio_manager_load_sound(&engine->audio_manager, "assets/example.wav", "test");

}


void check_collision(minigl_engine *engine, minigl_obj *obj){
    int x = 0;
    int y = 1;
    minigl_scene *current_scene = engine->scenes[engine->current_scene];
    //printf("object X Y = (%f, %f)\n", obj->position[x], obj->position[y]);

    for (int i = 0; i < current_scene->object_count; i++){
        minigl_obj *current_obj = current_scene->objects[i];

        if (current_obj == obj) {
            continue;
        }

        int overlap_x = (obj->position[x] < current_obj->position[x] + current_obj->size[x] && 
                          obj->position[x] + obj->size[x] > current_obj->position[x]);

        int overlap_y = (obj->position[y] < current_obj->position[y] + current_obj->size[y] && 
                          obj->position[y] + obj->size[y] > current_obj->position[y]);

        if (overlap_x && overlap_y) {
            
            float overlap_right = (current_obj->position[x] + current_obj->size[x]) - obj->position[x]; 
            float overlap_left = (obj->position[x] + obj->size[x]) - current_obj->position[x];          
            float overlap_down = (current_obj->position[y] + current_obj->size[y]) - obj->position[y];  
            float overlap_up = (obj->position[y] + obj->size[y]) - current_obj->position[y];            

            if (overlap_left < overlap_right && overlap_left < overlap_down && overlap_left < overlap_up) {
                minigl_obj_set_position(obj, current_obj->position[x] - obj->size[x], obj->position[y]);
                obj->velocity[x] = 0;
            } else if (overlap_right < overlap_left && overlap_right < overlap_down && overlap_right < overlap_up) {
                minigl_obj_set_position(obj, current_obj->position[x] + current_obj->size[x], obj->position[y]);
                obj->velocity[x] = 0;
            } else if (overlap_up < overlap_down && overlap_up < overlap_left && overlap_up < overlap_right) {
                minigl_obj_set_position(obj, obj->position[x], current_obj->position[y] - obj->size[y]);
                obj->velocity[y] = 0;
            } else {
                minigl_obj_set_position(obj, obj->position[x], current_obj->position[y] + current_obj->size[y]);
                obj->velocity[y] = 0;
            }
        }
    }
}