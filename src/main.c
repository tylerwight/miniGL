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
    mat4 projection_matrix;
    glm_ortho(-200.0f, 200.0f, -150.0f, 150.0f, -1.0f, 1.0f, projection_matrix);

    //setup shaders
    shader main_shader;
    shader_create(&main_shader, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    shader_set_uniform_3f(&main_shader, "uniformColor", 0.1f, 0.5f, 0.1f);
    shader_set_uniform_1i(&main_shader, "uniformTexture", 0);
    shader_set_uniform_mat4f(&main_shader, "uniformMVP", projection_matrix);
    

    //Create quad vertex data
    int vertices_count = 8;
    quad quad1;
    quad_create(-20.0f, -20.0f, 40.0f, &quad1);
    

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
    renderable_object square;
    renderable_object_create(&square, &quad1, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, &texture);


    //main loop
    while (!glfwWindowShouldClose(window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderable_object_draw(&square);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    renderable_object_delete(&square);
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