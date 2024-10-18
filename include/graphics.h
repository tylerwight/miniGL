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
#include <signal.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


#define MAX_VERTX_ATTRIBS 4



typedef struct shader {
    GLuint program;
    const char* vertex_source;
    const char* fragment_source;
} shader;


typedef struct buffer {
    GLuint id;
    GLenum type;
    void* data;
    size_t length;
} buffer;

typedef struct vertex_attrib_pointer{
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    GLvoid* pointer;
    GLint pointer_offset;
    
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
    vertex_array *vao;
    buffer *vbo;
    buffer *ibo;
    shader *shader;
    texture *texture;
    mat4 model_matrix;
} renderable_object;


typedef struct color{
    float r;
    float g;
    float b;
    float a;
} color;

typedef struct vertex{
    float position[2];
    float text_coords[2];
    float text_slot;
    float color[4];
} vertex;

typedef struct quad{
    struct vertex v0, v1, v2, v3;
} quad;


typedef struct renderer{
    renderable_object *objects;
    int object_count;
    vertex_array *vao;
    buffer *vbo;
    buffer *ibo;
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
void shader_delete(shader *input);

//buffers
void buffer_create(buffer *input, GLenum type, void *data, size_t length);
void buffer_bind(buffer *input);
void buffer_unbind(buffer *input);
void buffer_update(buffer *input);
void buffer_update_text_slot(buffer *input, float id);
void buffer_delete(buffer *input);
void buffer_set_data(buffer *input, void *data, size_t length);

//vertex arrays
void vertex_array_create(vertex_array *input);
void vertex_array_bind(vertex_array *input);
void vertex_array_unbind();
vertex_attrib_pointer vertex_array_attribute_create(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer);
void vertex_array_attribute_add(vertex_attrib_pointer *attributes, int attribute_index, GLenum type, int count);
void vertex_array_delete(vertex_array *input);

//renderable Object functions
void renderable_object_link(renderable_object *input, vertex_array *vao, buffer *vbo, buffer *ibo, shader *shader);
void renderable_object_draw(renderable_object *input);
void renderable_object_delete(renderable_object *input);
void renderable_object_create(renderable_object *input, void *vertices, int vertices_count, GLuint indices[], int indices_count, vertex_attrib_pointer attributes[], int attribute_count, shader *shader, texture *texture);
void renderable_object_print(renderable_object *input, const char* name);
void renderable_object_update_vertices(renderable_object *input, void *data, int vertices_count);

//renderer
void renderer_update_data(renderer *input);
void renderer_attach_object(renderer *input, renderable_object *object);
void renderer_initialize(renderer *input);
void renderer_draw(renderer *input);


//textures
void texture_load(texture *input, const char *path);
void texture_delete(texture *input);
void texture_bind(texture *input, int slot);
void texture_unbind();

//color
void color_set(color* dest, float r, float g, float b, float a);

//vertices setup
void vertex_print(vertex *input);
void quad_update_pos(quad *dest, float x, float y, int size);
void quad_create(quad *dest, float x, float y, int size, color color, float texture_id);
void vertices_print(vertex *input, int count);

void indices_print(void *data, int indices_count);

//batching
//void batch_add($)



GLFWwindow* setup_opengl(int resolution_x, int resolution_y, void (*key_callback)(GLFWwindow*, int, int, int, int) );
void opengl_set_default_state();

bool GLLogCall(const char* function, const char* file, int line);
void GLClearError();
void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam);
#endif
