#include "minigl.h"
#include "game.h"


#define PHYSICS_TIMESTEP 0.01666666 // 1/60 of second

//todo
//fix under collision bug
//fix memory leak
//add kill block
//add art
//add sounds
//refactor


int main(){
    minigl_engine *CHASAM_CHASER = minigl_init(1024.0f, 768.0f, "miniGL");
    load_assets(CHASAM_CHASER);

    minigl_scene *scene_menu = create_scene_menu(CHASAM_CHASER);
    minigl_scene *level1 = create_scene_level1(CHASAM_CHASER);
    minigl_scene *level2 = create_scene_level2(CHASAM_CHASER); 

    minigl_engine_attach_scene(CHASAM_CHASER, scene_menu);
    minigl_engine_attach_scene(CHASAM_CHASER, level1);
    minigl_engine_attach_scene(CHASAM_CHASER, level2);

    CHASAM_CHASER->current_scene = 0;


    audio_source *music_source = audio_manager_create_source(&CHASAM_CHASER->audio_manager, "sneksong");
    audio_manager_play_source(music_source, 1);

    double prev_time = glfwGetTime();
    CHASAM_CHASER->accumulated_frame_time = 0.0;
    int frame_count = 0;
    double fps_last_time = prev_time;

    //main loop
    while (!glfwWindowShouldClose(CHASAM_CHASER->window)){ // game loop
        update_input(&(CHASAM_CHASER->engine_input_manager));
        glfwPollEvents();
        
        double current_time = glfwGetTime();
        double frame_time = current_time - prev_time;
        prev_time = current_time;
        CHASAM_CHASER->accumulated_frame_time += frame_time;

        minigl_scene_run_loop(CHASAM_CHASER);
        minigl_draw(CHASAM_CHASER);
        
        //put FPS in title bar
        frame_count++;
        if (current_time - fps_last_time >= 1.0) {
            int fps = frame_count;
            char title[256];
            snprintf(title, sizeof(title), "miniGL - [FPS: %d]", fps);
            glfwSetWindowTitle(CHASAM_CHASER->window, title);
            frame_count = 0;
            fps_last_time = current_time;
        }

        audio_manager_update(&CHASAM_CHASER->audio_manager);
        glfwSwapBuffers(CHASAM_CHASER->window);
    }


    glfwTerminate();
    audio_manager_cleanup(&CHASAM_CHASER->audio_manager);
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



