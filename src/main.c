#include "graphics.h"
#include "menus.h"
#include "audio.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



int main(){
    GLFWwindow* window;
    window = setup_opengl(1024,768, key_callback);
    
    //setup shaders
    shader main_shader;
    view_projection_create(main_shader.view_projection, 1024.0f, 768.0f);
    shader_create(&main_shader, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    shader_set_uniform_mat4f(&main_shader, "uniform_view_projection", main_shader.view_projection);
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
    

    //Create quad vertex + index data
    int vertices_count = 4;
    quad quad_red, quad_green, quad_blue, quad_test;

    quad_create(&quad_red, 100.0f, 150.0f, 150.0f, red, -1.0f);
    quad_create(&quad_green, 400.0f, 150.0f, 100.0f, green, -1.0f);
    quad_create(&quad_blue, 700.0f, 150.0f, 50.0f, blue, -1.0f);
    quad_create(&quad_test, 700.0f, 350.0f, 50.0f, blue, -1.0f);
    

    int indicies_count = 6;
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    
    // new attributes testing
    int attribute_count = 4;
    vertex_attrib_pointer attributes[attribute_count];
    vertex_array_attribute_add(attributes, 0, GL_FLOAT, 2); // position
    vertex_array_attribute_add(attributes, 1, GL_FLOAT, 2); // texture position
    vertex_array_attribute_add(attributes, 2, GL_FLOAT, 1); // texture slot/id
    vertex_array_attribute_add(attributes, 3, GL_FLOAT, 4); // color


    // create some objects
    renderable_object square_red, square_green, square_blue, square_test;
    renderable_object_create(&square_red, &quad_red, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, NULL);
    renderable_object_create(&square_green, &quad_green, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, &texture_head);
    renderable_object_create(&square_blue, &quad_blue, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, NULL);
    renderable_object_create(&square_test, &quad_test, vertices_count, indices, indicies_count, attributes, attribute_count, &main_shader, &texture_body);

    //link them to a renderer
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
    glfwSetWindowUserPointer(window, game_renderer);

    
    double previousTime = glfwGetTime();
    int frameCount = 0;

    //main loop
    while (!glfwWindowShouldClose(window)){ // game loop
        
        glfwPollEvents();
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
  
    
        nuklear_debug_menu_draw(window, &debug_menu, &debug_menu_data);
        //renderer_translate(game_renderer, 0.0f, 0.5f);
        renderable_object_translate(&square_test, 0.5f, 0.0f);
        renderable_object_draw(&square_test);
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
        
    }


    renderable_object_delete(&square_red);
    glfwTerminate();
    return 0;
}





void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    renderer *game_renderer = glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
      if (action == GLFW_PRESS) {
          //move = 1;
      } else {
          //move = 0;
      }

        switch (key) {
            case GLFW_KEY_UP:
                renderer_translate(game_renderer, 0.0f, 10.0f);
                break;
            case GLFW_KEY_DOWN:
                renderer_translate(game_renderer, 0.0f, -10.0f);
                break;
            case GLFW_KEY_LEFT:
                renderer_translate(game_renderer, -10.0f, 0.0f);
                break;
            case GLFW_KEY_RIGHT:
                renderer_translate(game_renderer, 10.0f, 0.0f);
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }
}


