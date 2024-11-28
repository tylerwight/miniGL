#ifndef GAME_H
#define GAME_H
#include "minigl.h"


//COLLISION
void check_collision(minigl_engine *engine, minigl_obj *obj);
int is_colliding(minigl_engine *engine, minigl_obj *obj);
int is_colliding_top(minigl_engine *engine, minigl_obj *obj);
int is_colliding_between(minigl_obj *obj1, minigl_obj *obj2);
int is_outside_screen(minigl_engine *engine, minigl_obj *obj);

//MOVEMENT/INPUT
void process_input_generic(minigl_engine *minigl_engine, minigl_obj *obj);
void process_input_menu(minigl_engine *minigl_engine, minigl_obj *obj);
void process_input_lvledit(minigl_engine *minigl_engine, minigl_obj *obj);

//LOADING
void load_assets(minigl_engine *engine);

//SCENES
minigl_scene *create_scene_menu(minigl_engine *engine);
void scene_menu_loop(minigl_engine *engine);

minigl_scene *create_scene_level1(minigl_engine *engine);
minigl_scene *create_scene_level2(minigl_engine *engine);
minigl_scene *create_scene_level3(minigl_engine *engine);
minigl_scene *create_scene_win(minigl_engine *engine);

void scene_gameplay_loop(minigl_engine *engine);


minigl_scene *create_scene_lvledit(minigl_engine *engine);
void scene_lvledit_loop(minigl_engine *engine);




#endif