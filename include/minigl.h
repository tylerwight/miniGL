#ifndef MINIGL_H
#define MINIGL_H
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "menus.h"

struct minigl_engine;
struct minigl_scene;

typedef enum{
    MINIGL_STATIC,
    MINIGL_DYNAMIC,
    MINIGL_AUDIO,
    MINIGL_INVIS,
    MINIGL_EMPTY,
    MINIGL_BACKGROUND
} minigl_obj_type;

typedef struct{
    float position[2];
    float velocity[2];
    int size[2]; //width/height
    renderable_object *renderable;
    minigl_obj_type type;
} minigl_obj;

typedef struct{
    minigl_obj **objects;
    int object_count;
    void (*scene_loop)(struct minigl_engine *engine);
} minigl_scene;

typedef struct {
    input_manager engine_input_manager;
    renderer engine_renderer;
    GLFWwindow *window;
    float window_x;
    float window_y;
    minigl_scene **scenes;
    int scene_count;
    int current_scene;
    audio_manager audio_manager; 
    double accumulated_frame_time;
} minigl_engine;



minigl_obj *minigl_obj_create_quad(minigl_engine *engine, float x_pos, float y_pos, int width, int height, color color, const char *texture_name, const char *shader_name, minigl_obj_type type);
void minigl_obj_edit_quad(minigl_engine *engine, float x_pos, float y_pos, int width, int height, color color, const char *texture_name, const char *shader_name, minigl_obj_type type, minigl_obj *input_obj);
void minigl_obj_set_position(minigl_obj *obj, float x_pos, float y_pos);
int minigl_obj_compare(const void *a, const void *b);

void minigl_shader_load(minigl_engine *engine, const char* vertex_shader_path, const char* fragment_shader_path, const char* name);
shader *minigl_shader_get_by_name(minigl_engine *engine, const char* name);

void minigl_texture_load(minigl_engine *engine, const char *texture_path, const char *name);
texture* minigl_texture_get_by_name(minigl_engine *engine, const char *name);

minigl_scene* minigl_scene_create();
void minigl_scene_attach_object(minigl_scene *scene, minigl_obj *object);
void minigl_scene_attach_object_many(minigl_scene *scene, minigl_obj *object[], int obj_count);
void minigl_engine_attach_scene(minigl_engine *engine, minigl_scene *scene);
void minigl_scene_run_loop(minigl_engine *engine);
void minigl_scene_draw(minigl_engine *engine, minigl_scene *scene);

minigl_engine *minigl_init(float x, float y, const char * name);
void minigl_process_movement(minigl_engine *engine);
void minigl_draw(minigl_engine *engine);

#endif