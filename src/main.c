#include "minigl.h"
#include "game.h"


#define PHYSICS_TIMESTEP 0.01666666 // 1/60 of second

//todo
//fix under collision bug
//fix memory leak
//add art
//add sounds
//create 3 levels

//done
//refactor
//add kill block


int main(){
    minigl_engine *CHASAM_JUMP = minigl_init(1024.0f, 768.0f, "miniGL");
    load_assets(CHASAM_JUMP);

    minigl_scene *scene_menu = create_scene_menu(CHASAM_JUMP);
    minigl_scene *level1 = create_scene_level1(CHASAM_JUMP);
    minigl_scene *level2 = create_scene_level2(CHASAM_JUMP); 
    minigl_scene *level3 = create_scene_level3(CHASAM_JUMP); 
    minigl_scene *scene_win = create_scene_win(CHASAM_JUMP); 

    minigl_engine_attach_scene(CHASAM_JUMP, scene_menu);
    minigl_engine_attach_scene(CHASAM_JUMP, level1);
    minigl_engine_attach_scene(CHASAM_JUMP, level2);
    minigl_engine_attach_scene(CHASAM_JUMP, level3);
    minigl_engine_attach_scene(CHASAM_JUMP, scene_win);

    CHASAM_JUMP->current_scene = 0;


    audio_source *music_source = audio_manager_create_source(&CHASAM_JUMP->audio_manager, "sneksong");
    //audio_manager_play_source(music_source, 1);

    double prev_time = glfwGetTime();
    CHASAM_JUMP->accumulated_frame_time = 0.0;
    int frame_count = 0;
    double fps_last_time = prev_time;

    //main loop
    while (!glfwWindowShouldClose(CHASAM_JUMP->window)){ // game loop
        update_input(&(CHASAM_JUMP->engine_input_manager));
        glfwPollEvents();
        
        double current_time = glfwGetTime();
        double frame_time = current_time - prev_time;
        prev_time = current_time;
        CHASAM_JUMP->accumulated_frame_time += frame_time;


        if (CHASAM_JUMP->accumulated_frame_time >= PHYSICS_TIMESTEP) { //update viewport when resized
            int window_w;
            int window_h;
            glfwGetWindowSize(CHASAM_JUMP->window, &window_w, &window_h);
            glViewport(0, 0, window_w, window_h);
        }

        minigl_scene_run_loop(CHASAM_JUMP);
        minigl_draw(CHASAM_JUMP);
        
        //put FPS in title bar
        frame_count++;
        if (current_time - fps_last_time >= 1.0) {

            int fps = frame_count;
            char title[256];
            snprintf(title, sizeof(title), "miniGL - [FPS: %d]", fps);
            glfwSetWindowTitle(CHASAM_JUMP->window, title);
            frame_count = 0;
            fps_last_time = current_time;
        }

        audio_manager_update(&CHASAM_JUMP->audio_manager);
        glfwSwapBuffers(CHASAM_JUMP->window);
    }


    glfwTerminate();
    audio_manager_cleanup(&CHASAM_JUMP->audio_manager);
    return 0;
}







    // struct nuklear_container nuklear_cont;
    // struct nuklear_debug_menu debug_data;
    // nuklear_container_setup(myapp->window, &nuklear_cont);
    // nuklear_debug_menu_setup(&debug_data, "Debug", 50.0f, 50.0f, 50.0f, 50.0f);
    // glfwSetWindowUserPointer(myapp->window, myapp);
    // debug_data.friction = &friction;
    // debug_data.speed = &speed;
    // debug_data.gravity = &gravity;
    // debug_data.jump_speed = &jump_speed;



