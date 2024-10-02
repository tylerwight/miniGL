#ifndef GRAPHICS_H
#define GRAPHICS_H
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h> 
#include <cglm/cam.h> 
#include <cglm/affine.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OBJECTS 1000



typedef struct shader {
    GLuint program;
    const char* vertex_source;
    const char* fragment_source;
} shader;


typedef struct buffer {
    GLuint id;
    GLenum type;
    GLsizeiptr size;
    void* data;
} buffer;


typedef struct texture {
    GLuint id;
    int width;
    int height;
    unsigned char* data;
} texture;

typedef struct renderable_object {
    GLuint vao;
    buffer vbo;
    buffer ibo;
    shader *shader;
    texture *texture;
    mat4 model_matrix;
    GLsizei index_count;
} renderable_object;


typedef struct renderer {
    renderable_object *objects[MAX_OBJECTS];
    int object_count;
} renderer;





//renderer functions





//shader functions
const char* load_shader_source(const char* file_path);
void create_shader_program(shader *shader);
GLuint compile_shader(GLenum type, const char* source);

#endif