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



#endif