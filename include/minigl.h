#ifndef MINIGL_H
#define MINIGL_H
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "menus.h"

typedef struct {
    input_manager engine_input_manager;
    renderer engine_renderer;
    GLFWwindow *window;
} engine;

typedef struct{
    float position[2];
    float velocity[2];
    renderable_object *renderable;
    
} minigl_obj;

typedef struct{
    minigl_obj **objects;
    int object_count;
} scene;


engine *minigl_init(float x, float y, const char * name);


#endif