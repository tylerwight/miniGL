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


GLFWwindow* setup_opengl(int resolution_x, int resolution_y){
    GLFWwindow* window;
    if (!glfwInit()){exit(-1);}

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(resolution_x, resolution_y, "Window", NULL, NULL);
        if (!window){
            glfwTerminate();
            exit(-1);
        }


        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
        {
            printf("Failed to initialize GLEW\n");
            exit(-1);
        }
        glfwSetKeyCallback(window, key_callback);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // probabaly not a good idea, but not sure how to change in freetype to align (yet)
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        return window;
}



int main(){
    GLFWwindow* window;
    window = setup_opengl(1024,768);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f, // bottom right
        0.5f, 0.5f, 0.0f, // top right
        -0.5f, 0.5f, 0.0f, // top left
    };

    GLuint indices[] = {
        0, 2, 1,
        3, 2, 0
    };

    //setup shaders
    shader main_shader;
    main_shader.vertex_source = load_shader_source("shaders/vertex_shader.glsl");
    main_shader.fragment_source = load_shader_source("shaders/fragment_shader.glsl");
    create_shader_program(&main_shader);
    glUseProgram(main_shader.program);


    buffer quad_VBO, quad_IBO;
    vertex_array quad_VAO;
    renderable_object square;
printf("made it here");
    vertex_array_create(&quad_VAO);
    buffer_create(&quad_VBO, GL_ARRAY_BUFFER, vertices, sizeof(float)*12);
    buffer_create(&quad_IBO, GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(GLuint)*6);
    
    quad_VAO.attributes[0] = vertex_array_attribute_create(&quad_VAO, 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    renderable_object_create(&square, &quad_VAO, &quad_VBO, &quad_IBO, &main_shader);


    // vertex_array_bind(&quad_VAO);
    // buffer_bind(&quad_VBO);
    // buffer_bind(&quad_IBO);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // vertex_array_unbind(&quad_VAO);
    // buffer_unbind(&quad_VBO);
    // buffer_unbind(&quad_IBO);


    // for (int i = 0; i < 12; i++){
    //     printf("float%d: %f\n", i, ((float*)quad_VBO.data)[i]);
    // }
    // for (int i = 0; i < 6; i++){
    //     printf("int%d: %d\n", i, ((GLuint*)quad_IBO.data)[i]);
    // }


    
    // unsigned int VBO;
    // unsigned int IBO;
    // unsigned int VAO;
    
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glGenBuffers(1, &IBO);
    
    // glBindVertexArray(VAO);
    
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, vertices, GL_DYNAMIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, indices, GL_DYNAMIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    
    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    while (!glfwWindowShouldClose(window)){ // game loop
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //vertex_array_bind(&quad_VAO);
        //glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        draw_renderable_object(&square);


        glfwSwapBuffers(window);
    }


    // glDeleteVertexArrays(OBJECT_COUNT, game.VAO);
    // glDeleteBuffers(OBJECT_COUNT, game.VBO);
    // glDeleteProgram(game.shader_program_quads);

    glfwTerminate();
    return 0;


}


