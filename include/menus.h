#ifndef MENUS_H
#define MENUS_H
#include "graphics.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

struct nuklear_container{
    struct nk_glfw glfw;
    int width;
    int height;
    struct nk_context *ctx;
    struct nk_colorf bg; 
};

struct nuklear_debug_menu{
    const char *name;
    float object_pos1[2];
    float object_pos2[2];
};

void nuklear_container_setup(GLFWwindow *window, struct nuklear_container *input);
void nuklear_debug_menu_setup(struct nuklear_debug_menu *input, const char *name, float x1, float y1, float x2, float y2);
void nuklear_debug_menu_draw(GLFWwindow *window, struct nuklear_container *container, struct nuklear_debug_menu *data);


#endif