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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


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
    //shader_set_uniform_1i(&main_shader, "uniformTexture", 0);
    shader_set_uniform_mat4f(&main_shader, "uniformMVP", mvp);
    int samplers[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    shader_set_uniform_1iv(&main_shader, "u_textures", 10, samplers);
    

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
    //glBindTextureUnit(1, texture_head.id);
    //glBindTextureUnit(2, texture_body.id);
    

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
    //renderable_object_print(&square_red, "SQUARE RED");
    printf("attrib count (MAIN) %d\n", square_red.vao.attribute_count);

    renderer renderer;

    renderer.objects[0] = square_red;
    renderer.objects[1] = square_green;
    renderer.objects[2] = square_blue;
    renderer_inintalize(&renderer);
    

    //main loop
    while (!glfwWindowShouldClose(window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // renderable_object_draw(&square_red);
        // renderable_object_draw(&square_green);
        // renderable_object_draw(&square_blue);
        renderer_draw(&renderer);

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