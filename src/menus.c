#include "menus.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"


void nuklear_container_setup(GLFWwindow *window, struct nuklear_container *input){
    memset(&input->glfw, 0, sizeof(input->glfw));
    input->width = 0;
    input->height = 0;

    input->ctx = nk_glfw3_init(&input->glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&input->glfw, &atlas);
    nk_glfw3_font_stash_end(&input->glfw);
    }

    input->bg.r = 0.10f, input->bg.g = 0.18f, input->bg.b = 0.24f, input->bg.a = 1.0f;
}

void nuklear_debug_menu_setup(struct nuklear_debug_menu *input, const char *name, float x1, float y1, float x2, float y2){
    input->name = strdup(name);
    input->object_pos1[0] = x1;
    input->object_pos1[1] = y1;
    input->object_pos2[0] = x2;
    input->object_pos2[1] = y2;
}

void nuklear_debug_menu_draw(GLFWwindow *window, struct nuklear_container *container, struct nuklear_debug_menu *data){
        nk_glfw3_new_frame(&container->glfw);

        /* GUI */
        if (nk_begin(container->ctx, data->name, nk_rect(50, 50, 230, 300),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {

            nk_layout_row_dynamic(container->ctx, 20, 1);
            nk_label(container->ctx, "A X axis", NK_TEXT_LEFT);
            nk_layout_row_dynamic(container->ctx, 25, 1);
            nk_slider_float(container->ctx, 1.0f, &(data->object_pos1[0]), 1024.0f, 1.0f);
            nk_layout_row_dynamic(container->ctx, 20, 1);
            nk_label(container->ctx, "A Y axis", NK_TEXT_LEFT);
            nk_layout_row_dynamic(container->ctx, 25, 1);
            nk_slider_float(container->ctx, 1.0f, &(data->object_pos1[1]), 768.0f, 1.0f);
            nk_layout_row_dynamic(container->ctx, 20, 1);
            nk_label(container->ctx, "B X axis", NK_TEXT_LEFT);
            nk_layout_row_dynamic(container->ctx, 25, 1);
            nk_slider_float(container->ctx, 1.0f, &(data->object_pos2[0]), 1024.0f, 1.0f);
            nk_layout_row_dynamic(container->ctx, 20, 1);
            nk_label(container->ctx, "B Y axis", NK_TEXT_LEFT);
            nk_layout_row_dynamic(container->ctx, 25, 1);
            nk_slider_float(container->ctx, 1.0f, &(data->object_pos2[1]), 768.0f, 1.0f);
            nk_layout_row_dynamic(container->ctx, 25, 1);

        }
        nk_end(container->ctx);


        glfwGetWindowSize(window, &container->width, &container->height);
        glViewport(0, 0, container->width, container->height);
        nk_glfw3_render(&container->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        opengl_set_default_state();
}