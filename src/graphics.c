#include "graphics.h"


GLuint shader_compile(GLenum type, const char* source){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", info_log);
    }
    return shader;
}

void shader_create_program(shader *shader){
    GLuint vertex_shader = shader_compile(GL_VERTEX_SHADER, shader->vertex_source);
    GLuint fragment_shader = shader_compile(GL_FRAGMENT_SHADER, shader->fragment_source);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("ERROR::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    shader->program =  shader_program;
    return;
}

const char* shader_load_source(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", file_path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Could not allocate memory for file: %s\n", file_path);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

void shader_create(shader *input, const char *vertex_path, const char *fragment_path){
    input->vertex_source = shader_load_source(vertex_path);
    input->fragment_source = shader_load_source(fragment_path);
    shader_create_program(input);

}

shader_delete(shader *input){
    glDeleteProgram(input->program);
}


void buffer_create(buffer *input, GLenum type, void *data, size_t length){
    glGenBuffers(1, &(input->id));

    input->data = malloc(length);
    memcpy(input->data, data, length);

    input->type = type;
    input->length = length;

    buffer_update(input);
}

void buffer_update(buffer *input){
    buffer_bind(input);
    glBufferData(input->type, input->length, input->data, GL_DYNAMIC_DRAW);
    buffer_unbind(input);
}

void buffer_bind(buffer *input){
    glBindBuffer(input->type, input->id);
}
void buffer_unbind(buffer *input){
    glBindBuffer(input->type, 0);
}

void buffer_delete(buffer *input){
    glDeleteBuffers(1, &(input->id));
    free(input->data);
}


void vertex_array_create(vertex_array *input){
    glGenVertexArrays(1, &(input->id));
    input->attribute_count = 0;
}

void vertex_array_bind(vertex_array *input){
    glBindVertexArray(input->id);
}
void vertex_array_unbind(vertex_array *input){
    glBindVertexArray(0);
}

void vertex_array_delete(vertex_array *input){
    glDeleteVertexArrays(1, &(input->id));
}

vertex_attrib_pointer vertex_array_attribute_create(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer){
    vertex_attrib_pointer attribute;
    attribute.index = index;
    attribute.size = size;
    attribute.type = type;
    attribute.normalized = normalized; 
    attribute.stride = stride;
    attribute.pointer = pointer;

    return attribute;

}

void renderable_object_create(renderable_object *input, vertex_array *vao, buffer *vbo, buffer *ibo, shader *shader){
    input->vao = *vao;
    input->vbo = *vbo;
    input->ibo = *ibo;
    input->shader = shader;
    //input->texture = texture;
    vertex_array_bind(vao);
    buffer_bind(vbo);
    buffer_bind(ibo);
    for (int i = 0; i < vao->attribute_count; i++){
        glVertexAttribPointer(i, vao->attributes[i].size, vao->attributes[i].type, vao->attributes[i].normalized, vao->attributes[i].stride, vao->attributes[i].pointer);
        glEnableVertexAttribArray(i);
    }

    vertex_array_unbind(vao);
    buffer_unbind(vbo);
    buffer_unbind(ibo);
    

}

void renderable_object_create2(renderable_object *input, float vertices[], int vertices_count, GLuint indices[], int indices_count, vertex_attrib_pointer attributes[], int attribute_count, shader *shader){
    input->shader = shader;
    vertex_array VAO;
    buffer VBO, IBO;
    vertex_array_create(&VAO);
    buffer_create(&VBO, GL_ARRAY_BUFFER, vertices, sizeof(float) * vertices_count);
    buffer_create(&IBO, GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(GLuint) * indices_count);

    vertex_array_bind(&VAO);
    buffer_bind(&VBO);
    buffer_bind(&IBO);

    for (int i = 0; i < attribute_count; i++){
        glVertexAttribPointer(i, attributes[i].size, attributes[i].type, attributes[i].normalized, attributes[i].stride, attributes[i].pointer);
        glEnableVertexAttribArray(i);
    }

    vertex_array_unbind(&VAO);
    buffer_unbind(&VBO);
    buffer_unbind(&IBO);

}


void renderable_object_draw(renderable_object *input){
    vertex_array_bind(&(input->vao));
    glUseProgram(input->shader->program);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}

void renderable_object_delete(renderable_object *input){
    vertex_array_delete(&(input->vao));
    buffer_delete(&(input->ibo));
    buffer_delete(&(input->vbo));
    shader_delete(input->shader);
}

GLFWwindow* setup_opengl(int resolution_x, int resolution_y, void (*key_callback)(GLFWwindow*, int, int, int, int) ){
    GLFWwindow* window;
    if (!glfwInit()){exit(-1);}

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(resolution_x, resolution_y, "Snek", NULL, NULL);
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