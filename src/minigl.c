#include "minigl.h"




////////////////////
////////objects//////
////////////////////

minigl_obj *minigl_obj_create_quad(minigl_engine *engine, float x_pos, float y_pos, int width, int height, color color, const char *texture_name, const char *shader_name, minigl_obj_type type){
    minigl_obj *output_obj = calloc(1, sizeof(minigl_obj));
    output_obj->renderable = calloc(1, sizeof(renderable_object));

    texture *obj_texture = minigl_texture_get_by_name(engine, texture_name);
    shader *obj_shader = minigl_shader_get_by_name(engine, shader_name);

    quad *obj_quad = quad_create(x_pos, y_pos, width, height,  color, obj_texture);
    
    renderable_object_create_fromquad(output_obj->renderable, obj_quad, obj_shader);

    if (type == MINIGL_STATIC){
        output_obj->renderable->type = RO_STATIC;
    } else {
        output_obj->renderable->type = RO_DYNAMIC;
    }


    output_obj->position[0] = x_pos;
    output_obj->position[1] = y_pos;
    output_obj->size[0] = width;
    output_obj->size[1] = height;
    output_obj->type = type;
    return output_obj;

}



void minigl_obj_edit_quad(minigl_engine *engine, float x_pos, float y_pos, int width, int height, color color, const char *texture_name, const char *shader_name, minigl_obj_type type, minigl_obj *input_obj){


    texture *obj_texture = minigl_texture_get_by_name(engine, texture_name);
    shader *obj_shader = minigl_shader_get_by_name(engine, shader_name);

    quad *obj_quad = quad_create(x_pos, y_pos, width, height,  color, obj_texture);

    renderable_object_create_fromquad(input_obj->renderable, obj_quad, obj_shader);

    free(obj_quad);

    if (type == MINIGL_STATIC){
        input_obj->renderable->type = RO_STATIC;
    } else {
        input_obj->renderable->type = RO_DYNAMIC;
    }


    input_obj->position[0] = x_pos;
    input_obj->position[1] = y_pos;
    input_obj->size[0] = width;
    input_obj->size[1] = height;
    input_obj->type = type;

    

}

void minigl_obj_set_position(minigl_obj *obj, float x_pos, float y_pos){
    float x_delta = x_pos - obj->position[0];
    float y_delta = y_pos - obj->position[1];
    renderable_object_translate(obj->renderable, x_delta, y_delta);
    obj->position[0] = x_pos;
    obj->position[1] = y_pos;
}

int minigl_obj_compare(const void *a, const void *b) {
    const minigl_obj *obj_a = *(const minigl_obj **)a;
    const minigl_obj *obj_b = *(const minigl_obj **)b;

    // Define the order
    int type_priority[] = {
        [MINIGL_BACKGROUND] = 0,
        [MINIGL_STATIC] = 1,
        [MINIGL_DYNAMIC] = 2,
        [MINIGL_AUDIO] = 3, 
        [MINIGL_INVIS] = 4, 
        [MINIGL_EMPTY] = 5  
    };

    int priority_a = type_priority[obj_a->type];
    int priority_b = type_priority[obj_b->type];

    return (priority_a - priority_b);
}



////////////////////
////////shaders//////
////////////////////
void minigl_shader_load(minigl_engine *engine, const char* vertex_shader_path, const char* fragment_shader_path, const char* name){
    engine->engine_renderer.shaders = realloc(engine->engine_renderer.shaders, sizeof(shader*) * (engine->engine_renderer.shader_count + 1));

    shader *new_shader = initialize_shader(engine->window_x, engine->window_y, vertex_shader_path, fragment_shader_path);
    strncpy(new_shader->name, name, sizeof(new_shader->name));

    engine->engine_renderer.shaders[engine->engine_renderer.shader_count] = new_shader;
    engine->engine_renderer.shader_count++;


    shader_set_uniform_mat4f(new_shader, "uniform_projection", engine->engine_renderer.cam.projection_matrix);
    shader_set_uniform_mat4f(new_shader, "uniform_view", engine->engine_renderer.cam.view_matrix);

}

shader *minigl_shader_get_by_name(minigl_engine *engine, const char* name) {
    for (int i = 0; i < engine->engine_renderer.shader_count; i++) {
        if (strcmp(engine->engine_renderer.shaders[i]->name, name) == 0) {
            return engine->engine_renderer.shaders[i];
        }
    }
    return NULL;  // Return NULL if shader is not found
}


////////////////////
////////textures//////
////////////////////

void minigl_texture_load(minigl_engine *engine, const char *texture_path, const char *name) {
    engine->engine_renderer.textures = realloc(engine->engine_renderer.textures, sizeof(texture*) * (engine->engine_renderer.texture_count + 1));
    
    texture *new_texture = texture_load(texture_path);
    strncpy(new_texture->name, name, sizeof(new_texture->name));

    engine->engine_renderer.textures[engine->engine_renderer.texture_count] = new_texture;
    engine->engine_renderer.texture_count++;
}

texture* minigl_texture_get_by_name(minigl_engine *engine, const char *name) {
    if (name == NULL){
        return NULL;
    }
    for (int i = 0; i < engine->engine_renderer.texture_count; i++) {
        if (strcmp(engine->engine_renderer.textures[i]->name, name) == 0) {
            return engine->engine_renderer.textures[i];
        }
    }
    return NULL;
}


//scenes

minigl_scene* minigl_scene_create() {
    minigl_scene *new_scene = malloc(sizeof(minigl_scene));

    new_scene->objects = NULL;
    new_scene->object_count = 0;

    return new_scene;
}

void minigl_scene_attach_object(minigl_scene *scene, minigl_obj *object) {
    scene->objects = realloc(scene->objects, sizeof(minigl_obj*) * (scene->object_count + 1));

    scene->objects[scene->object_count] = object;
    scene->object_count++;
}


void minigl_scene_attach_object_many(minigl_scene *scene, minigl_obj *object[], int obj_count) {
    scene->objects = realloc(scene->objects, sizeof(minigl_obj*) * (scene->object_count + obj_count));


    for (int i = 0; i < obj_count; i++){
        scene->objects[scene->object_count] = object[i];
        scene->object_count++;
    }

}

void minigl_engine_attach_scene(minigl_engine *engine, minigl_scene *scene) {
    engine->scenes = realloc(engine->scenes, sizeof(minigl_scene*) * (engine->scene_count + 1));

    engine->scenes[engine->scene_count] = scene;
    engine->scene_count++;
}

void minigl_scene_run_loop(minigl_engine *engine){
    minigl_scene *current_scene = engine->scenes[engine->current_scene];
    current_scene->scene_loop(engine);
}

// void minigl_scene_draw(minigl_engine *engine, minigl_scene *scene) {
//     renderable_object **objects = calloc(scene->object_count, sizeof(renderable_object*));
    
//     for (int i = 0; i < scene->object_count; i++){
//         objects[i] = scene->objects[i]->renderable;
//     }
//     renderer_draw(&engine->engine_renderer, objects, scene->object_count);
//     free(objects);
// }



void minigl_scene_draw(minigl_engine *engine, minigl_scene *scene) {
    // Create an array of minigl_obj pointers for sorting
    minigl_obj **obj_array = calloc(scene->object_count, sizeof(minigl_obj *));
    for (int i = 0; i < scene->object_count; i++) {
        obj_array[i] = scene->objects[i];
    }

    // Sort the array of objects based on type priority
    qsort(obj_array, scene->object_count, sizeof(minigl_obj *), minigl_obj_compare);

    // Create an array of renderable_object pointers
    //printf("=====\n");
    renderable_object **objects = calloc(scene->object_count, sizeof(renderable_object *));
    for (int i = 0; i < scene->object_count; i++) {
        objects[i] = obj_array[i]->renderable;
        
        // if (obj_array[i]->type == MINIGL_BACKGROUND){
        //     printf("background \n");
        // }
        // if (obj_array[i]->type == MINIGL_STATIC){
        //     printf("static \n");
        // }
        // if (obj_array[i]->type == MINIGL_DYNAMIC){
        //     printf("DYNAMIC \n");
        // }
        
    }
    //printf("=====\n");

    // Draw using the renderer
    renderer_draw(&engine->engine_renderer, objects, scene->object_count);

    // Clean up
    free(obj_array);
    free(objects);
}


//engine
minigl_engine *minigl_init(float x, float y, const char * name){
    minigl_engine *output_engine = calloc(1, sizeof(minigl_engine));
    GLFWwindow* window;
    window = setup_opengl(x, y, name);
    output_engine->window_x = x;
    output_engine->window_y = y;


    output_engine->window = window;

    initialize_input(window);
    glfwSetWindowUserPointer(window, output_engine);


    output_engine->engine_renderer.cam = camera_create(1024.0f, 768.0f);
    output_engine->engine_renderer.current_shader = -1; 
    output_engine->audio_manager = *audio_manager_init(); 
    return output_engine;
}


void minigl_process_movement(minigl_engine *engine){
    minigl_scene *current_scene = engine->scenes[engine->current_scene];
    if (current_scene == NULL){
        printf("MPM NO CURRENT SCENE\n");
    }

    for (int i = 0; i < current_scene->object_count; i++){
        minigl_obj *current_object = current_scene->objects[i];

        if (current_object == NULL){
            printf("MPM NO OBJECT FOUND\n");
        }
        float new_x = current_object->position[0] + current_object->velocity[0];
        float new_y = current_object->position[1] + current_object->velocity[1];
        minigl_obj_set_position(current_object, new_x, new_y);
    }
}





void minigl_draw(minigl_engine *engine){
    int current_scene_index = engine->current_scene;
    minigl_scene *current_scene = engine->scenes[current_scene_index];
    minigl_scene_draw(engine, current_scene);
}

