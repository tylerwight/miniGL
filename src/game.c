#include "game.h"

#define PHYSICS_TIMESTEP 0.01666666 // 1/60 of second
int jumping = 0;
float friction = 0.8f;
float speed = 5.0f;
float gravity = -2.0f;
float jump_speed = 29.0f;

//////////////////////////
//////COLLISION////////////
/////////////////////////
void check_collision(minigl_engine *engine, minigl_obj *obj){
    int x = 0;
    int y = 1;
    minigl_scene *current_scene = engine->scenes[engine->current_scene];

    for (int i = 0; i < current_scene->object_count; i++){
        minigl_obj *current_obj = current_scene->objects[i];
        

        if (current_obj == obj || current_obj->type == MINIGL_BACKGROUND) {
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

            if (overlap_up < overlap_down && overlap_up < overlap_left && overlap_up < overlap_right) { //collide top
                if (obj->velocity[y] > -25.0f){
                    minigl_obj_set_position(obj, obj->position[x], current_obj->position[y] - obj->size[y]);
                } else {
                    minigl_obj_set_position(obj, obj->position[x], current_obj->position[y] + current_obj->size[y]);
                }

                obj->velocity[y] = 0.0f;
            } else if (overlap_left < overlap_right && overlap_left < overlap_down && overlap_left < overlap_up) { //collide left
                minigl_obj_set_position(obj, current_obj->position[x] - obj->size[x], obj->position[y]);
                obj->velocity[x] = 0.0f;

            } else if (overlap_right < overlap_left && overlap_right < overlap_down && overlap_right < overlap_up) { //collide right
                minigl_obj_set_position(obj, current_obj->position[x] + current_obj->size[x], obj->position[y]);
                obj->velocity[x] = 0.0f;

            } else {                                                                                            //collide bottom
                minigl_obj_set_position(obj, obj->position[x], current_obj->position[y] + current_obj->size[y]);
                obj->velocity[y] = 0.0f;
            }
        }
    }
}


int is_colliding(minigl_engine *engine, minigl_obj *obj) {
    int x = 0;
    int y = 1;
    minigl_scene *current_scene = engine->scenes[engine->current_scene];

    for (int i = 0; i < current_scene->object_count; i++) {
        minigl_obj *current_obj = current_scene->objects[i];

        if (current_obj == obj) {
            continue;
        }

        int overlap_x = (obj->position[x] < current_obj->position[x] + current_obj->size[x] && 
                         obj->position[x] + obj->size[x] > current_obj->position[x]);

        int overlap_y = (obj->position[y] < current_obj->position[y] + current_obj->size[y] && 
                         obj->position[y] + obj->size[y] > current_obj->position[y]);

        if (overlap_x && overlap_y) {
            return 1; 
        }
    }

    return 0; // No collision
}

int is_colliding_top(minigl_engine *engine, minigl_obj *obj) {
    int x = 0;
    int y = 1;
    minigl_scene *current_scene = engine->scenes[engine->current_scene];

    for (int i = 0; i < current_scene->object_count; i++) {
        minigl_obj *current_obj = current_scene->objects[i];

        // Skip if the object is itself
        if (current_obj == obj) {
            continue;
        }

        // Check for horizontal overlap
        int overlap_x = (obj->position[x] < current_obj->position[x] + current_obj->size[x] && 
                         obj->position[x] + obj->size[x] > current_obj->position[x]);

        // Check if obj's bottom aligns with current_obj's top
        int top_collision = (obj->position[y] == current_obj->position[y] + current_obj->size[y]);

        // Only return true if both conditions are met
        if (overlap_x && top_collision) {
            return 1; // Collision detected on the top of the object
        }
    }

    return 0; // No top collision
}

int is_colliding_between(minigl_obj *obj1, minigl_obj *obj2) {
    int x = 0;
    int y = 1;

    // Check for overlap in the x-axis
    int overlap_x = (obj1->position[x] < obj2->position[x] + obj2->size[x] && 
                     obj1->position[x] + obj1->size[x] > obj2->position[x]);

    // Check for overlap in the y-axis
    int overlap_y = (obj1->position[y] < obj2->position[y] + obj2->size[y] && 
                     obj1->position[y] + obj1->size[y] > obj2->position[y]);

    // Return 1 if both x and y axes overlap, indicating a collision
    return overlap_x && overlap_y;
}


int is_outside_screen(minigl_engine *engine, minigl_obj *obj) {
    int x = 0;
    int y = 1;

    // Get the window dimensions from the engine
    float screen_width = engine->window_x + 20;
    float screen_height = engine->window_y + 20;
    float lower_bound = -20;

    // Check if the object is out of bounds in any direction
    if (obj->position[x] + obj->size[x] < lower_bound ||        // Left side
        obj->position[x] > screen_width ||            // Right side
        obj->position[y] + obj->size[y] < lower_bound ||        // Bottom side
        obj->position[y] > screen_height) {           // Top side
        return 1; // The object is outside the screen
    }

    return 0; // The object is within screen bounds
}




//////////////////////////
//////MOVEMENT/INPUT////////////
/////////////////////////
void process_input_generic(minigl_engine *minigl_engine, minigl_obj *obj){
    int x = 0;
    int y = 1;
    static int wait = 0;
    
    //printf("speed: %f\nfriction : %f\ngravity: %f\n jumping: %d\n VelX %f\n VelY %f\n", speed, friction, gravity, jumping, obj->velocity[0], obj->velocity[1]);
    //printf("X: %f, Y: %f\n", obj->position[x], obj->position[y]);

    

    if (obj->velocity[x] > 0.0f){
        if (obj->velocity[x] < friction){
            obj->velocity[x] = 0.0f;
        }else{
            obj->velocity[x] = obj->velocity[x] - friction;
        }
    }
    if (obj->velocity[x] < 0.0f){
        if (obj->velocity[x] > friction){
            obj->velocity[x] = 0.0f;
        }else{
            obj->velocity[x] = obj->velocity[x] + friction;
        }
    }
    if (obj->velocity[y] > 0.0f){
        if (obj->velocity[y] < friction){
            obj->velocity[y] = 0.0f;
        }else{
            obj->velocity[y] = obj->velocity[y] - friction;
        }
    }
    if (obj->velocity[y] < 0.0f){
        if (obj->velocity[y] > friction){
            obj->velocity[y] = 0.0f;
        }else{
            obj->velocity[y] = obj->velocity[y] + friction;
        }
    }

    if (obj->velocity[x] > -1.0f && obj->velocity[x] < 1.0f){ obj->velocity[x] = 0.0f;}
    if (obj->velocity[y] > -1.0f && obj->velocity[y] < 1.0f){ obj->velocity[y] = 0.0f;}

    if (is_colliding_top(minigl_engine, obj)){
        jumping = 0;
        obj->velocity[y] = 0;
    } else{
        obj->velocity[y] += gravity;
    }

    if (is_outside_screen(minigl_engine, obj)){
        if (minigl_engine->current_scene == 3){
            minigl_obj_set_position(obj, 25, 75);
        } else{
            minigl_obj_set_position(obj, 25, 100);
        }
        
    }
    

    
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(minigl_engine->window, GLFW_TRUE);
    }

    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_SPACE) || is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_W) || is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_UP)) {
        if (jumping == 0){
            jumping = 1;
            obj->velocity[y] = jump_speed;
            wait = 20;
            
        }

        if (jumping == 1 && wait == 0){
            jumping = 2;
            obj->velocity[y] = jump_speed;
        }

    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_A) || is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_LEFT)) {
        obj->velocity[x] = -speed;
    }
    // if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_S)) {
    //     //obj->velocity[y] = -speed;
    // }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_D) || is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_RIGHT)) {
        obj->velocity[x] = speed;
    }

    if (wait > 0){wait--;};
}

void process_input_menu(minigl_engine *minigl_engine, minigl_obj *obj){
    int x = 0;
    int y = 1;
    static int wait = 0;
    
    //printf("speed: %f\nfriction : %f\ngravity: %f\n jumping: %d\n VelX %f\n VelY %f\n", speed, friction, gravity, jumping, obj->velocity[0], obj->velocity[1]);
    //printf("X: %f, Y: %f\n", obj->position[x], obj->position[y]);

    

    if (obj->velocity[x] > 0.0f){
        if (obj->velocity[x] < friction){
            obj->velocity[x] = 0.0f;
        }else{
            obj->velocity[x] = obj->velocity[x] - friction;
        }
    }
    if (obj->velocity[x] < 0.0f){
        if (obj->velocity[x] > friction){
            obj->velocity[x] = 0.0f;
        }else{
            obj->velocity[x] = obj->velocity[x] + friction;
        }
    }
    if (obj->velocity[y] > 0.0f){
        if (obj->velocity[y] < friction){
            obj->velocity[y] = 0.0f;
        }else{
            obj->velocity[y] = obj->velocity[y] - friction;
        }
    }
    if (obj->velocity[y] < 0.0f){
        if (obj->velocity[y] > friction){
            obj->velocity[y] = 0.0f;
        }else{
            obj->velocity[y] = obj->velocity[y] + friction;
        }
    }

    if (obj->velocity[x] > -1.0f && obj->velocity[x] < 1.0f){ obj->velocity[x] = 0.0f;}
    if (obj->velocity[y] > -1.0f && obj->velocity[y] < 1.0f){ obj->velocity[y] = 0.0f;}

    if (is_colliding_top(minigl_engine, obj)){
        jumping = 0;
    } else{
        obj->velocity[y] += gravity;
    }

    if (is_outside_screen(minigl_engine, obj)){
        minigl_obj_set_position(obj, 512, 700);
    }
    

    
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(minigl_engine->window, GLFW_TRUE);
    }

    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_SPACE) || is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_W) || is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_UP)) {
        if (jumping == 0){
            jumping = 1;
            obj->velocity[y] = jump_speed;
            wait = 20;
            
        }

        if (jumping == 1 && wait == 0){
            jumping = 2;
            obj->velocity[y] = jump_speed;
        }

    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_A) || is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_LEFT)) {
        obj->velocity[x] = -speed;
    }
    // if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_S)) {
    //     //obj->velocity[y] = -speed;
    // }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_D) || is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_RIGHT)) {
        obj->velocity[x] = speed;
    }

    if (wait > 0){wait--;};
}



void process_input_lvledit(minigl_engine *minigl_engine, minigl_obj *obj){
    int x = 0;
    int y = 1;
    static int wait = 0;
    
    //printf("speed: %f\nfriction : %f\ngravity: %f\n jumping: %d\n VelX %f\n VelY %f\n", speed, friction, gravity, jumping, obj->velocity[0], obj->velocity[1]);
    //printf("X: %f, Y: %f\n", obj->position[x], obj->position[y]);
    float friction_lvledit = 0.8f;
    float speed_lvledit = 2.0f;
    int resize_speed = 5;
    

    if (obj->velocity[x] > 0.0f){
        if (obj->velocity[x] < friction_lvledit){
            obj->velocity[x] = 0.0f;
        }else{
            obj->velocity[x] = obj->velocity[x] - friction_lvledit;
        }
    }
    if (obj->velocity[x] < 0.0f){
        if (obj->velocity[x] > friction_lvledit){
            obj->velocity[x] = 0.0f;
        }else{
            obj->velocity[x] = obj->velocity[x] + friction_lvledit;
        }
    }
    if (obj->velocity[y] > 0.0f){
        if (obj->velocity[y] < friction_lvledit){
            obj->velocity[y] = 0.0f;
        }else{
            obj->velocity[y] = obj->velocity[y] - friction_lvledit;
        }
    }
    if (obj->velocity[y] < 0.0f){
        if (obj->velocity[y] > friction_lvledit){
            obj->velocity[y] = 0.0f;
        }else{
            obj->velocity[y] = obj->velocity[y] + friction_lvledit;
        }
    }

    if (obj->velocity[x] > -1.0f && obj->velocity[x] < 1.0f){ obj->velocity[x] = 0.0f;}
    if (obj->velocity[y] > -1.0f && obj->velocity[y] < 1.0f){ obj->velocity[y] = 0.0f;}

    



    
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(minigl_engine->window, GLFW_TRUE);
    }

    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_A)) {
        obj->velocity[x] = -speed_lvledit;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_S)) {
        obj->velocity[y] = -speed_lvledit;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_D)) {
        obj->velocity[x] = speed_lvledit;
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_W)) {
        obj->velocity[y] = speed_lvledit;
    }

    color red, green, blue;
    color_set(&red, 1.0f, 0.4f, 0.0f, 1.0f);
    color_set(&green, 0.4f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.4f, 0.0f, 1.0f, 1.0f);

    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_UP)) {
        minigl_obj_edit_quad(minigl_engine, obj->position[0], obj->position[1], obj->size[0], obj->size[1]+resize_speed, green, NULL , "mainshader", MINIGL_DYNAMIC, obj);
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_DOWN)) {
        minigl_obj_edit_quad(minigl_engine, obj->position[0], obj->position[1], obj->size[0], obj->size[1]-resize_speed, green, NULL , "mainshader", MINIGL_DYNAMIC, obj);
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_LEFT)) {
        minigl_obj_edit_quad(minigl_engine, obj->position[0], obj->position[1], obj->size[0]-resize_speed, obj->size[1], green, NULL , "mainshader", MINIGL_DYNAMIC, obj);
    }
    if (is_key_down(&minigl_engine->engine_input_manager, GLFW_KEY_RIGHT)) {
        minigl_obj_edit_quad(minigl_engine, obj->position[0], obj->position[1], obj->size[0]+resize_speed, obj->size[1], green, NULL , "mainshader", MINIGL_DYNAMIC, obj);
    }


}




//////////////////////////
//////LOADING////////////
/////////////////////////
void load_assets(minigl_engine *engine){
    minigl_shader_load(engine, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", "mainshader");
    minigl_texture_load(engine, "assets/snek_head.png", "head");
    minigl_texture_load(engine, "assets/snek_body1.png", "body1");
    minigl_texture_load(engine, "assets/title.png", "title");
    minigl_texture_load(engine, "assets/win.png", "win");
    audio_manager_load_sound(&engine->audio_manager, "assets/example.wav", "test");
    audio_manager_load_sound(&engine->audio_manager, "assets/sneksong.wav", "sneksong");
    audio_manager_load_sound(&engine->audio_manager, "assets/eat_snek.wav", "eat");
    audio_manager_load_sound(&engine->audio_manager, "assets/ouch_snek.wav", "ouch");


}



//////////////////////////
//////SCENES////////////
/////////////////////////
minigl_scene *create_scene_menu(minigl_engine *engine){
    //create some colors
    color red, green, blue;
    color_set(&red, 1.0f, 0.4f, 0.0f, 1.0f);
    color_set(&green, 0.4f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.4f, 0.0f, 1.0f, 1.0f);

    int object_count = 3;
    minigl_obj *objects[object_count];
    objects[0] = minigl_obj_create_quad(engine, 512.0f, 700.0f, 25, 25, green, NULL , "mainshader", MINIGL_DYNAMIC);
    objects[1] = minigl_obj_create_quad(engine, 250.0f, 50.0f, 500, 100, blue, NULL, "mainshader", MINIGL_STATIC);
    objects[2] = minigl_obj_create_quad(engine, 0.0f, 0.0f, 1024, 768, red, "title", "mainshader", MINIGL_BACKGROUND);


    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object_many(scene, objects, object_count);

    scene->scene_loop = scene_menu_loop;

    return scene;
}






minigl_scene *create_scene_level1(minigl_engine *engine){
    //create some colors
    color red, green, blue, purple, yellow;
    color_set(&red, 1.0f, 0.0f, 0.0f, 1.0f);
    color_set(&green, 0.0f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.0f, 0.0f, 1.0f, 1.0f);
    color_set(&purple, 0.5f, 0.0f, 0.5f, 1.0f);
    color_set(&yellow, 1.0f, 1.0f, 0.0f, 1.0f);

    int object_count = 11;
    minigl_obj *objects[object_count];
    objects[0] = minigl_obj_create_quad(engine, 25.0f, 250.0f, 25, 25, green, NULL , "mainshader", MINIGL_DYNAMIC); // player
    objects[1] = minigl_obj_create_quad(engine, 0.0f, 50.0f, 200, 100, red, NULL, "mainshader", MINIGL_STATIC);
    objects[2] = minigl_obj_create_quad(engine, 700.0f, 50.0f, 300, 100, red, NULL, "mainshader", MINIGL_STATIC);
    objects[3] = minigl_obj_create_quad(engine, 250.0f, 200.0f, 50, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[4] = minigl_obj_create_quad(engine, 375.0f, 350.0f, 50, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[5] = minigl_obj_create_quad(engine, 0.0f, 50.0f, 25, 700, red, NULL, "mainshader", MINIGL_STATIC);
    objects[6] = minigl_obj_create_quad(engine, 1000.0f, 50.0f, 25, 700, red, NULL, "mainshader", MINIGL_STATIC);
    objects[7] = minigl_obj_create_quad(engine, 0.0f, 700.0f, 1024, 200, blue, NULL, "mainshader", MINIGL_STATIC);
    objects[8] = minigl_obj_create_quad(engine, 850.0f, 150.0f, 25, 25, purple, NULL, "mainshader", MINIGL_STATIC);
    objects[9] = minigl_obj_create_quad(engine, 300.0f, 100.0f, 25, 450, yellow, NULL, "mainshader", MINIGL_STATIC);
    objects[10] = minigl_obj_create_quad(engine, 25.0f, 400.0f, 50, 25, red, NULL, "mainshader", MINIGL_STATIC);

    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object_many(scene, objects, object_count);
    scene->scene_loop = scene_gameplay_loop;
    //scene->scene_loop = scene_lvledit_loop;
    return scene;
}

minigl_scene *create_scene_level2(minigl_engine *engine){
    //create some colors
    color red, green, blue, purple, yellow;
    color_set(&red, 1.0f, 0.4f, 0.0f, 1.0f);
    color_set(&green, 0.4f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.4f, 0.0f, 1.0f, 1.0f);
    color_set(&purple, 0.5f, 0.0f, 0.5f, 1.0f);
    color_set(&yellow, 1.0f, 1.0f, 0.0f, 1.0f);

    int object_count = 15;
    minigl_obj *objects[object_count];
    objects[0] = minigl_obj_create_quad(engine, 10.0f, 150.0f, 25, 25, green, NULL , "mainshader", MINIGL_DYNAMIC);
    objects[1] = minigl_obj_create_quad(engine, 0.0f, 0.0f, 300, 50, red, NULL, "mainshader", MINIGL_STATIC);
    objects[2] = minigl_obj_create_quad(engine, 700.0f, 0.0f, 340, 50, red, NULL, "mainshader", MINIGL_STATIC);
    objects[3] = minigl_obj_create_quad(engine, 480.0f, 15.0f, 45, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[4] = minigl_obj_create_quad(engine, 340.0f, 350.0f, 50, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[5] = minigl_obj_create_quad(engine, 0.0f, 50.0f, 25, 700, red, NULL, "mainshader", MINIGL_STATIC);
    objects[6] = minigl_obj_create_quad(engine, 1000.0f, 50.0f, 25, 700, red, NULL, "mainshader", MINIGL_STATIC);
    objects[7] = minigl_obj_create_quad(engine, 0.0f, 735.0f, 1024, 200, blue, NULL, "mainshader", MINIGL_STATIC);
    objects[8] = minigl_obj_create_quad(engine, 950.0f, 650.0f, 25, 25, purple, NULL, "mainshader", MINIGL_STATIC);
    objects[9] = minigl_obj_create_quad(engine, 25.0f, 250.0f, 900, 35, yellow, NULL, "mainshader", MINIGL_STATIC);
    objects[10] = minigl_obj_create_quad(engine, 975.0f, 275.0f, 25, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[11] = minigl_obj_create_quad(engine, 650.0f, 285.0f, 60, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[12] = minigl_obj_create_quad(engine, 175.0f, 605.0f, 825, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[13] = minigl_obj_create_quad(engine, 25.0f, 518.0f, 150, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[14] = minigl_obj_create_quad(engine, 551.0f, 630.0f, 25, 25, yellow, NULL, "mainshader", MINIGL_STATIC);


    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object_many(scene, objects, object_count);
    scene->scene_loop = scene_gameplay_loop;

    return scene;
}


minigl_scene *create_scene_lvledit(minigl_engine *engine){
    //create some colors
    color red, green, blue, purple, yellow;
    color_set(&red, 1.0f, 0.4f, 0.0f, 1.0f);
    color_set(&green, 0.4f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.4f, 0.0f, 1.0f, 1.0f);
    color_set(&purple, 0.5f, 0.0f, 0.5f, 1.0f);
    color_set(&yellow, 1.0f, 1.0f, 0.0f, 1.0f);

    int object_count = 1;
    minigl_obj *objects[object_count];
    objects[0] = minigl_obj_create_quad(engine, 10.0f, 350.0f, 25, 25, green, NULL , "mainshader", MINIGL_DYNAMIC);



    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object_many(scene, objects, object_count);
    scene->scene_loop = scene_lvledit_loop;

    return scene;
}


minigl_scene *create_scene_level3(minigl_engine *engine){
    //create some colors
    color red, green, blue, purple, yellow;
    color_set(&red, 1.0f, 0.2f, 0.2f, 1.0f);
    color_set(&green, 0.2f, 1.0f, 0.2f, 1.0f);
    color_set(&blue, 0.2f, 0.2f, 1.0f, 1.0f);
    color_set(&purple, 0.5f, 0.0f, 0.5f, 1.0f);
    color_set(&yellow, 1.0f, 1.0f, 0.0f, 1.0f);

    int object_count = 17;
    minigl_obj *objects[object_count];
    objects[0] = minigl_obj_create_quad(engine, 25.0f, 75.0f, 25, 25, green, NULL , "mainshader", MINIGL_DYNAMIC);
    objects[1] = minigl_obj_create_quad(engine, 0.0f, 50.0f, 100, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[2] = minigl_obj_create_quad(engine, 25.0f, 125.0f, 245, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[3] = minigl_obj_create_quad(engine, 960.0f, 150.0f, 40, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[4] = minigl_obj_create_quad(engine, 605.0f, 00.0f, 440, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[5] = minigl_obj_create_quad(engine, 0.0f, 50.0f, 25, 700, red, NULL, "mainshader", MINIGL_STATIC);
    objects[6] = minigl_obj_create_quad(engine, 1000.0f, 25.0f, 25, 725, red, NULL, "mainshader", MINIGL_STATIC);
    objects[7] = minigl_obj_create_quad(engine, 0.0f, 750.0f, 1024, 200, blue, NULL, "mainshader", MINIGL_STATIC);
    objects[8] = minigl_obj_create_quad(engine, 32.0f, 661.0f, 25, 25, purple, NULL, "mainshader", MINIGL_STATIC);
    objects[9] = minigl_obj_create_quad(engine, 885.0f, 60.0f, 25, 600, yellow, NULL, "mainshader", MINIGL_STATIC);
    objects[10] = minigl_obj_create_quad(engine, 960.0f, 275.0f, 40, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[11] = minigl_obj_create_quad(engine, 960.0f, 400.0f, 40, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[12] = minigl_obj_create_quad(engine, 960.0f, 525.0f, 40, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[13] = minigl_obj_create_quad(engine, 960.0f, 650.0f, 40, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[14] = minigl_obj_create_quad(engine, 685.0f, 535.0f, 200, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[15] = minigl_obj_create_quad(engine, 195.0f, 575.0f, 25, 25, red, NULL, "mainshader", MINIGL_STATIC);
    objects[16] = minigl_obj_create_quad(engine, 315.0f, 435.0f, 25, 25, red, NULL, "mainshader", MINIGL_STATIC);


    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object_many(scene, objects, object_count);
    scene->scene_loop = scene_gameplay_loop;

    return scene;
}


minigl_scene *create_scene_win(minigl_engine *engine){
    //create some colors
    color red, green, blue;
    color_set(&red, 1.0f, 0.4f, 0.0f, 1.0f);
    color_set(&green, 0.4f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.4f, 0.0f, 1.0f, 1.0f);

    int object_count = 3;
    minigl_obj *objects[object_count];
    objects[0] = minigl_obj_create_quad(engine, 512.0f, 700.0f, 25, 25, green, NULL , "mainshader", MINIGL_DYNAMIC);
    objects[1] = minigl_obj_create_quad(engine, 250.0f, 50.0f, 500, 100, blue, NULL, "mainshader", MINIGL_STATIC);
    objects[2] = minigl_obj_create_quad(engine, 0.0f, 0.0f, 1024, 768, red, "win", "mainshader", MINIGL_BACKGROUND);


    minigl_scene *scene = minigl_scene_create();

    minigl_scene_attach_object_many(scene, objects, object_count);

    scene->scene_loop = scene_menu_loop;

    return scene;
}






//////////////////////////
//////SCENE LOOPS////////////
/////////////////////////
void scene_menu_loop(minigl_engine *engine){
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (is_key_down(&engine->engine_input_manager, GLFW_KEY_ENTER)) {
        engine->current_scene = 1;
        audio_manager_play_by_name(&engine->audio_manager, "eat");
    }

    while (engine->accumulated_frame_time >= PHYSICS_TIMESTEP) {
        
        process_input_menu(engine, engine->scenes[engine->current_scene]->objects[0]);
        minigl_process_movement(engine, PHYSICS_TIMESTEP); // Call with fixed timestep
        //if (is_colliding_between(engine->scenes[1]->objects[0], engine->scenes[1]->objects[8])){engine->current_scene = 2;}

        check_collision(engine, engine->scenes[engine->current_scene]->objects[0]);
        engine->accumulated_frame_time -= PHYSICS_TIMESTEP;
    }

}



void scene_gameplay_loop(minigl_engine *engine){
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    while (engine->accumulated_frame_time >= PHYSICS_TIMESTEP) {

        if (is_key_down(&engine->engine_input_manager, GLFW_KEY_F12)) {
                engine->scenes[engine->current_scene]->scene_loop = scene_lvledit_loop;
        }

        process_input_generic(engine, engine->scenes[engine->current_scene]->objects[0]);
        minigl_process_movement(engine, PHYSICS_TIMESTEP); // Call with fixed timestep
        if (engine->current_scene == 1){
            if (is_colliding_between(engine->scenes[1]->objects[0], engine->scenes[1]->objects[8])){engine->current_scene = 2;}
            if (is_colliding_between(engine->scenes[1]->objects[0], engine->scenes[1]->objects[9])){minigl_obj_set_position(engine->scenes[1]->objects[0], 25.0f, 200.0f);}
        }
        if (engine->current_scene == 2){
            if (is_colliding_between(engine->scenes[2]->objects[0], engine->scenes[2]->objects[8])){engine->current_scene = 3;}
            if (is_colliding_between(engine->scenes[2]->objects[0], engine->scenes[2]->objects[9]) || is_colliding_between(engine->scenes[2]->objects[0], engine->scenes[2]->objects[14])){minigl_obj_set_position(engine->scenes[2]->objects[0], 25.0f, 200.0f);}
        }
        if (engine->current_scene == 3){
            if (is_colliding_between(engine->scenes[3]->objects[0], engine->scenes[3]->objects[8])){engine->current_scene = 4;}
            if (is_colliding_between(engine->scenes[3]->objects[0], engine->scenes[3]->objects[9])){minigl_obj_set_position(engine->scenes[3]->objects[0], 25.0f, 200.0f);}
        }


        check_collision(engine, engine->scenes[engine->current_scene]->objects[0]);
        engine->accumulated_frame_time -= PHYSICS_TIMESTEP;
    }

}


void scene_lvledit_loop(minigl_engine *engine){
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    while (engine->accumulated_frame_time >= PHYSICS_TIMESTEP) {
        
        if (is_key_down(&engine->engine_input_manager, GLFW_KEY_F11)) {
                color green;
                color_set(&green, 0.4f, 1.0f, 0.0f, 1.0f);
                minigl_obj_edit_quad(engine, engine->scenes[engine->current_scene]->objects[0]->position[0], engine->scenes[engine->current_scene]->objects[0]->position[1], 25, 25, green, NULL , "mainshader", MINIGL_DYNAMIC, engine->scenes[engine->current_scene]->objects[0]);
                engine->scenes[engine->current_scene]->scene_loop = scene_gameplay_loop;
        }

        if (is_key_down(&engine->engine_input_manager, GLFW_KEY_F10)){
            engine->current_scene++;
        }

        process_input_lvledit(engine, engine->scenes[engine->current_scene]->objects[0]);
        minigl_process_movement(engine, PHYSICS_TIMESTEP); // Call with fixed timestep
        printf("X: %f, Y: %f\n", engine->scenes[engine->current_scene]->objects[0]->position[0], engine->scenes[engine->current_scene]->objects[0]->position[1]);
        printf("Width: %d, Height: %d\n", engine->scenes[engine->current_scene]->objects[0]->size[0], engine->scenes[engine->current_scene]->objects[0]->size[1]);

        engine->accumulated_frame_time -= PHYSICS_TIMESTEP;
    }

}
