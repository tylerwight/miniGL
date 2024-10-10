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

    //projections
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
    shader_set_uniform_3f(&main_shader, "uniformColor", 0.1f, 0.5f, 0.1f);
    shader_set_uniform_1i(&main_shader, "uniformTexture", 0);
    shader_set_uniform_mat4f(&main_shader, "uniformMVP", mvp);
    

    //Create quad vertex data
    int vertices_count = 8;
    quad quad1, quad2;
    quad_create(300.0f, 300.0f, 100.0f, &quad1);
    quad_create(600.0f, 200.0f, 40.0f, &quad2);
    

    int indicies_count = 6;
    GLuint indices[] = {
        0, 2, 1,
        3, 2, 0
    };


    //Vertex attributes for the quad
    vertex_attrib_pointer attributes[2];
    int attribute_count = 2;
    attributes[0] = vertex_array_attribute_create(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    attributes[1] = vertex_array_attribute_create(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));



    //load textures
    texture texture;
    texture_load(&texture, "assets/snek_head.png");

    // create the object
    renderable_object square1, square2;
    renderable_object_create(&square1, &quad1, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, &texture);
    renderable_object_create(&square2, &quad2, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, &texture);


    //main loop
    while (!glfwWindowShouldClose(window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderable_object_draw(&square1);
        renderable_object_draw(&square2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    renderable_object_delete(&square1);
    renderable_object_delete(&square2);
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