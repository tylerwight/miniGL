#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#define CGLM_DEFINE_PRINTS

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
#include <time.h>


#ifdef _WIN32
    #include <intrin.h>
    #define ASSERT(x) if (!(x)) __debugbreak();
#else
    #define ASSERT(x) if (!(x)) raise(SIGTRAP);
#endif

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


#define MAX_VERTX_ATTRIBS 4
#define MAX_TEXTURE_SLOTS 10



typedef struct shader {
    GLuint program;
    const char* vertex_source;
    const char* fragment_source;
    char name[32];
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
    char name[32];
} texture;

typedef enum ro_type{
    RO_STATIC,
    RO_DYNAMIC
} ro_type;

typedef struct renderable_object {
    vertex_array *vao;
    buffer *vbo;
    buffer *ibo;
    shader *shader;
    texture *texture;
    mat4 model_matrix;
    ro_type type;
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


typedef struct quad_vertices{
    vertex v0, v1, v2, v3;
} quad_vertices;


typedef struct quad{
    quad_vertices vertices_data;
    int vertices_count;
    GLuint indices_data[6];
    int indices_count;
    vertex_attrib_pointer attributes[MAX_VERTX_ATTRIBS];
    int attribute_count;
    texture *texture;
} quad;


typedef struct renderable_batch{
    renderable_object **objects;
    int object_count;
    vertex_array *vao;
    buffer *vbo;
    buffer *ibo;
    mat4 model_matrix;
    texture *texture_slots[MAX_TEXTURE_SLOTS];
    int used_slots;
} renderable_batch;

typedef struct camera{
    mat4 view_matrix;
    mat4 projection_matrix;
} camera;


typedef struct renderer{
    renderable_object **objects;
    int object_count;
    renderable_batch **batches;
    int batch_count;
    camera cam;
    int camera_unchanged;
    shader **shaders;
    int shader_count;
    int current_shader;
    int shader_unchanged;

    texture **textures;
    int texture_count;

} renderer;



//shader functions
const char* shader_load_source(const char* file_path);
void shader_create_program(shader *shader);
GLuint shader_compile(GLenum type, const char* source);
void shader_create(shader *input, const char *vertex_path, const char *fragment_path);
shader *initialize_shader(float x_size, float y_size, const char* vertex_shader_path, const char* fragment_shader_path);
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
void renderable_object_create_fromquad(renderable_object *dst, quad *data, shader *shader);
void renderable_object_create(renderable_object *input, void *vertices, int vertices_count, GLuint indices[], int indices_count, vertex_attrib_pointer attributes[], int attribute_count, shader *shader, texture *texture);
void renderable_object_print(renderable_object *input, const char* name);
void renderable_object_update_vertices(renderable_object *input, void *data, int vertices_count);
void renderable_object_translate(renderable_object *input, float x, float y);


//renderable batch
renderable_batch *renderable_batch_create();
void renderable_batch_update_data(renderable_batch *input);
void renderable_batch_attach_object(renderable_batch *input, renderable_object *object);
void renderable_batch_initialize(renderable_batch *input);
void renderable_batch_translate(renderable_batch *input, float x, float y);
void renderable_batch_draw(renderable_batch *input);
void renderable_batch_flush(renderable_batch *input);


//renderer
void renderer_attach_object(renderer *dst, renderable_object *object);
void renderer_attach_batch(renderer *dst, renderable_batch *batch);

void renderer_draw(renderer *renderer, renderable_object **objects, int object_count);


//camera
camera camera_create(float x, float y);

//textures
texture *texture_load(const char *path);
void texture_delete(texture *input);
void texture_bind(texture *input, int slot);
void texture_unbind();

//color
void color_set(color* dest, float r, float g, float b, float a);

//vertices setup
void vertex_print(vertex *input);
void quad_update_pos(quad *dest, float x, float y, int size);
quad* quad_create(float x, float y, int width, int height, color color, texture *texture);
void vertices_print(vertex *input, int count);

void indices_print(void *data, int indices_count);

// view projection
void view_projection_create(mat4 dest, float x, float y);

//batching
//void batch_add($)



GLFWwindow* setup_opengl(int resolution_x, int resolution_y, const char *name);
void opengl_set_default_state();

bool GLLogCall(const char* function, const char* file, int line);
void GLClearError();
void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam);
#endif
