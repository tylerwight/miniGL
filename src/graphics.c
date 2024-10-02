#include "graphics.h"


GLuint compile_shader(GLenum type, const char* source){
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

void create_shader_program(shader *shader){
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, shader->vertex_source);
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, shader->fragment_source);

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

const char* load_shader_source(const char* file_path) {
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

vertex_attrib_pointer vertex_array_attribute_create(vertex_array *input, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer){
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
    //input->shader = shader;
    //input->texture = texture;
    vertex_array_bind(vao);
    buffer_bind(vbo);
    buffer_bind(ibo);
    //glVertexAttribPointer(vao->attributes[0].index, vao->attributes[0].size, vao->attributes[0].type, vao->attributes[0].normalized, vao->attributes[0].stride, vao->attributes[0].pointer);
    //glEnableVertexAttribArray(0);

    for (int i = 0; i < vao->attribute_count; i++){
        glVertexAttribPointer(i, vao->attributes[i].size, vao->attributes[0].type, vao->attributes[0].normalized, vao->attributes[0].stride, vao->attributes[0].pointer);
        glEnableVertexAttribArray(i);
    }

    vertex_array_unbind(vao);
    buffer_unbind(vbo);
    buffer_unbind(ibo);
    

}


void renderable_object_draw(renderable_object *input){
    vertex_array_bind(&(input->vao));
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}

void renderable_object_delete(renderable_object *input){
    vertex_array_delete(&(input->vao));
    buffer_delete(&(input->ibo));
    buffer_delete(&(input->vbo));
    shader_delete(input->shader);
}