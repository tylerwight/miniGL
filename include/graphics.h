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
#define MAX_VERTX_ATTRIBS 10



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
    size_t length;
    int indices_count;
} buffer;

typedef struct vertex_attrib_pointer{
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    GLvoid* pointer;
    
} vertex_attrib_pointer;

typedef struct vertex_array{
    GLuint id;
    vertex_attrib_pointer attributes[MAX_VERTX_ATTRIBS];
    int attribute_count;

} vertex_array;


typedef struct texture {
    GLuint id;
    int width;
    int height;
    unsigned char* data;
    char *path;
} texture;

typedef struct renderable_object {
    vertex_array vao;
    buffer vbo;
    buffer ibo;
    shader *shader;
    texture *texture;
    mat4 model_matrix;
    GLsizei index_count;
} renderable_object;


// typedef struct renderer {
//     renderable_object objects[MAX_OBJECTS];
//     int object_count;
// } renderer;

typedef struct color{
    float r;
    float g;
    float b;
    float a;
} color;

typedef struct vertex{
    float position[2];
    float text_coords[2];
    float text_index;
    float color[4];
} vertex;

typedef struct quad{
    struct vertex v0, v1, v2, v3;
} quad;


typedef struct renderer{
    renderable_object objects[3];
    vertex_array vao;
    buffer vbo;
    int vertex_data_length;
    buffer ibo;
    int indices_count;
    GLuint bound_textures[10];
    int current_texture_slot;
} renderer;




//shader functions
const char* shader_load_source(const char* file_path);
void shader_create_program(shader *shader);
GLuint shader_compile(GLenum type, const char* source);
void shader_create(shader *input, const char *vertex_path, const char *fragment_path);
void shader_set_uniform_1i(shader *shader, const char *name, int one);
void shader_set_uniform_3f(shader *shader, const char *name, float one, float two, float three);
void shader_set_uniform_4f(shader *shader, const char *name, float one, float two, float three, float four);
void shader_set_uniform_mat4f(shader *shader, const char *name, mat4 one);
void shader_set_uniform_1iv(shader *shader, const char *name, int count , int array[]);

//buffers
void buffer_create(buffer *input, GLenum type, void *data, size_t length);
void buffer_bind(buffer *input);
void buffer_unbind(buffer *input);
void buffer_update(buffer *input);
void buffer_update_text_id(buffer *input, float id);
void buffer_delete(buffer *input);

//vertex arrays
void vertex_array_create(vertex_array *input);
void vertex_array_bind(vertex_array *input);
void vertex_array_unbind(vertex_array *input);
vertex_attrib_pointer vertex_array_attribute_create(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer);
void vertex_array_delete(vertex_array *input);

//renderable Object functions
void renderable_object_link(renderable_object *input, vertex_array *vao, buffer *vbo, buffer *ibo, shader *shader);
void renderable_object_draw(renderable_object *input);
void renderable_object_delete(renderable_object *input);
void renderable_object_create(renderable_object *input, void *vertices, int vertices_count, GLuint indices[], int indices_count, vertex_attrib_pointer attributes[], int attribute_count, shader *shader, texture *texture);
void renderable_object_print(renderable_object *input, const char* name);

//renderer
void renderer_draw(renderer *input);
void renderer_inintalize(renderer *input);

//textures
void texture_load(texture *input, const char *path);
void texture_delete(texture *input);
void texture_bind(texture *input, int slot);
void texture_unbind(texture *input);

//color
void color_set(color* dest, float r, float g, float b, float a);

//vertices setup
void print_vertex(vertex *input);
void quad_create(quad *dest, float x, float y, int size, color color, float texture_id);

//batching
//void batch_add($)



GLFWwindow* setup_opengl(int resolution_x, int resolution_y, void (*key_callback)(GLFWwindow*, int, int, int, int) );


#endif
