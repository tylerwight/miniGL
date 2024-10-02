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
//#include "game.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main(){
    GLFWwindow* window;
    window = setup_opengl(1024,768, key_callback);

    //Test data to make a square:
    int vertices_count = 12;
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f, // bottom right
        0.5f, 0.5f, 0.0f, // top right
        -0.5f, 0.5f, 0.0f, // top left
    };
    int indicies_count = 6;
    GLuint indices[] = {
        0, 2, 1,
        3, 2, 0
    };

    //Vertex attributes for the square
    vertex_attrib_pointer attributes[1];
    int attribute_count = 1;
    attributes[0] = vertex_array_attribute_create(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    //setup shaders
    shader main_shader;
    shader_create(&main_shader, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    
    
    // create the object
    renderable_object square;
    renderable_object_create2(&square, vertices, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader);


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
                exit(0);
                break;
        }
    }
}