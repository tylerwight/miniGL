#include "minigl.h"


minigl_engine *minigl_init(float x, float y, const char * name){
    minigl_engine *output_engine = calloc(1, sizeof(minigl_engine));
    GLFWwindow* window;
    window = setup_opengl(x, y, name);
    output_engine->window_x = x;
    output_engine->window_y = y;


    output_engine->window = window;

    initialize_input(window);
    glfwSetWindowUserPointer(window, output_engine);
    
    return output_engine;
}

////////////////////
////////objects//////
////////////////////

minigl_obj *minigl_obj_create_quad(minigl_engine *engine, float x_pos, float y_pos, int size, color color, const char *texture_name, const char *shader_name){
    minigl_obj *output_obj = calloc(1, sizeof(minigl_obj));
    output_obj->renderable = calloc(1, sizeof(renderable_object));

    texture *obj_texture = minigl_texture_get_by_name(engine, texture_name);
    shader *obj_shader = minigl_shader_get_by_name(engine, shader_name);

    quad *obj_quad = quad_create(x_pos, y_pos, size, color, obj_texture);
    
    renderable_object_create_fromquad(output_obj->renderable, obj_quad, obj_shader);

    output_obj->position[0] = x_pos;
    output_obj->position[1] = y_pos;
    return output_obj;

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

// Function to load a texture into the renderer and assign it a name
void minigl_texture_load(minigl_engine *engine, const char *texture_path, const char *name) {
    engine->engine_renderer.textures = realloc(engine->engine_renderer.textures, sizeof(texture*) * (engine->engine_renderer.texture_count + 1));
    
    texture *new_texture = texture_load(texture_path);
    strncpy(new_texture->name, name, sizeof(new_texture->name));

    engine->engine_renderer.textures[engine->engine_renderer.texture_count] = new_texture;
    engine->engine_renderer.texture_count++;
}

// Function to find a texture by its path (name)
texture* minigl_texture_get_by_name(minigl_engine *engine, const char *name) {
    for (int i = 0; i < engine->engine_renderer.texture_count; i++) {
        if (strcmp(engine->engine_renderer.textures[i]->name, name) == 0) {
            return engine->engine_renderer.textures[i];
        }
    }
    return NULL;  // Return NULL if texture not found
}


//scenes

minigl_scene* minigl_scene_create() {
    // Allocate memory for the scene
    minigl_scene *new_scene = malloc(sizeof(minigl_scene));

    // Initialize the object array and set the object count to 0
    new_scene->objects = NULL; // No objects initially
    new_scene->object_count = 0;

    return new_scene;
}

void minigl_scene_attach_object(minigl_scene *scene, minigl_obj *object) {
    // Reallocate memory to hold one more object in the scene
    scene->objects = realloc(scene->objects, sizeof(minigl_obj*) * (scene->object_count + 1));

    // Add the object to the scene and increment the object count
    scene->objects[scene->object_count] = object;
    scene->object_count++;
}

void minigl_engine_attach_scene(minigl_engine *engine, minigl_scene *scene) {
    // Reallocate memory to hold one more scene in the engine
    engine->scenes = realloc(engine->scenes, sizeof(minigl_scene*) * (engine->scene_count + 1));

    // Attach the scene to the engine and increment the scene count
    engine->scenes[engine->scene_count] = scene;
    engine->scene_count++;
}


void minigl_scene_draw(minigl_engine *engine, minigl_scene *scene) {
    // Loop through all objects in the scene
    for (int i = 0; i < scene->object_count; i++) {
        minigl_obj *obj = scene->objects[i];
        renderable_object_draw(obj->renderable);
    }
}
