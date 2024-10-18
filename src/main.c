#define GLEW_STATIC

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"
#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h> 
#include <cglm/cam.h> 
#include <cglm/affine.h>
//#include "game.h"
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
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


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


int main(){
    GLFWwindow* window;
    window = setup_opengl(1024,768, key_callback);

    //Model View Projection
    mat4 projection;
    glm_ortho(0.0f, 1024.0f, 0.0f, 768.0f, -1.0f, 1.0f, projection);

    mat4 view;
    glm_mat4_identity(view);
    vec3 viewtranslation = {-100.f, 0.0f, 0.0f};
    glm_translate(view, viewtranslation);

    mat4 model;
    glm_mat4_identity(model);
    vec3 modeltranslation = {200, 200, 0};
    glm_translate(model, modeltranslation);

    mat4 mvp;
    mat4 temp;
    glm_mat4_mul(projection, view, temp);
    glm_mat4_mul(temp, model, mvp);


    //setup shaders
    shader main_shader;
    shader_create(&main_shader, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    shader_set_uniform_mat4f(&main_shader, "uniformMVP", mvp);
    int texture_slots[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    shader_set_uniform_1iv(&main_shader, "u_textures", 10, texture_slots);
    

    //create some colors
    color red, green, blue;
    color_set(&red, 1.0f, 0.0f, 0.0f, 1.0f);
    color_set(&green, 0.0f, 1.0f, 0.0f, 1.0f);
    color_set(&blue, 0.0f, 0.0f, 1.0f, 1.0f);
    

    //load textures
    texture texture_head;
    texture texture_body;
    texture_load(&texture_head, "assets/snek_head.png");
    texture_load(&texture_body, "assets/snek_body1.png");
    

    //Create quad vertex data
    int vertices_count = 4;
    quad quad_red, quad_green, quad_blue;

    quad_create(&quad_red, 100.0f, 150.0f, 150.0f, red, -1.0f);
    quad_create(&quad_green, 400.0f, 150.0f, 100.0f, green, -1.0f);
    quad_create(&quad_blue, 700.0f, 150.0f, 50.0f, blue, -1.0f);
    

    int indicies_count = 6;
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    //Vertex attributes for the quad
    int attribute_count = 4;
    vertex_attrib_pointer attributes[attribute_count];
    int stride = 9;

    attributes[0] = vertex_array_attribute_create(0, 2, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)0);                  // position
    attributes[1] = vertex_array_attribute_create(1, 2, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(2*sizeof(float)));  // texture position
    attributes[2] = vertex_array_attribute_create(2, 1, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(4*sizeof(float)));  // texture index
    attributes[3] = vertex_array_attribute_create(3, 4, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(5*sizeof(float)));  // color
    
    // create the object
    renderable_object square_red, square_green, square_blue;
    renderable_object_create(&square_red, &quad_red, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, NULL);
    renderable_object_create(&square_green, &quad_green, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, &texture_head);
    renderable_object_create(&square_blue, &quad_blue, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, NULL);

    renderer *game_renderer = calloc(1, sizeof(renderer));
    renderer_attach_object(game_renderer, &square_red);
    renderer_attach_object(game_renderer, &square_green);
    renderer_attach_object(game_renderer, &square_blue);
    renderer_initialize(game_renderer);    
    renderer_update_data(game_renderer);


    //nuklear setup
    struct nuklear_container debug_menu;
    struct nuklear_debug_menu debug_menu_data;
    nuklear_container_setup(window, &debug_menu);
    nuklear_debug_menu_setup(&debug_menu_data, "Debug Menu", 200.0f, 200.0f, 400.0f, 200.0f);


    
    double previousTime = glfwGetTime();
    int frameCount = 0;

    //main loop
    while (!glfwWindowShouldClose(window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
  
    
        nuklear_debug_menu_draw(window, &debug_menu, &debug_menu_data);

        opengl_set_default_state();
        
        quad_update_pos(&quad_red, debug_menu_data.object_pos1[0], debug_menu_data.object_pos1[1], 150);
        quad_update_pos(&quad_green, debug_menu_data.object_pos2[0], debug_menu_data.object_pos2[1], 100);
        renderable_object_update_vertices(&square_red, &quad_red, 4);
        renderable_object_update_vertices(&square_green, &quad_green, 4);


        renderer_update_data(game_renderer);
        renderer_draw(game_renderer);


        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - previousTime >= 1.0) {
            // Calculate FPS
            int fps = frameCount;
            char title[256];
            snprintf(title, sizeof(title), "My Game - [FPS: %d]", fps);
            glfwSetWindowTitle(window, title);
            frameCount = 0;
            previousTime = currentTime;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    renderable_object_delete(&square_red);
    glfwTerminate();
    return 0;
}





void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
      if (action == GLFW_PRESS) {
          //move = 1;
      } else {
          //move = 0;
      }

        switch (key) {
            case GLFW_KEY_UP:
                break;
            case GLFW_KEY_DOWN:
                break;
            case GLFW_KEY_LEFT:
                break;
            case GLFW_KEY_RIGHT:
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }
}