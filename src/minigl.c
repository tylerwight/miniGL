#include "minigl.h"


engine *minigl_init(float x, float y, const char * name){
    engine *output_engine = calloc(1,sizeof(engine));
    GLFWwindow* window;
    window = setup_opengl(x, y, name);

    output_engine->window = window;

    
    return output_engine;
}