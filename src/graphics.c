#include "graphics.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//////////////////////////
//////SHADERS////////////
/////////////////////////
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

void shader_set_uniform_1i(shader *shader, const char *name, int one){
    int tmp = glGetUniformLocation(shader->program, name);
    GLCall(glUseProgram(shader->program));
    GLCall(glUniform1i(tmp, one));
}

void shader_set_uniform_3f(shader *shader, const char *name, float one, float two, float three){
    int tmp = glGetUniformLocation(shader->program, name);
    GLCall(glUseProgram(shader->program));
    GLCall(glUniform3f(tmp, one, two, three));
}

void shader_set_uniform_4f(shader *shader, const char *name, float one, float two, float three, float four){
    int tmp = glGetUniformLocation(shader->program, name);
    GLCall(glUseProgram(shader->program));
    GLCall(glUniform4f(tmp, one, two, three, four));
}
void shader_set_uniform_mat4f(shader *shader, const char *name, mat4 one){
    int tmp = glGetUniformLocation(shader->program, name);
    GLCall(glUseProgram(shader->program));
    GLCall(glUniformMatrix4fv(tmp, 1, GL_FALSE, &one[0][0]));
}

void shader_set_uniform_1iv(shader *shader, const char *name, int count , int array[]){
    int tmp = glGetUniformLocation(shader->program, name);
    GLCall(glUseProgram(shader->program));
    GLCall(glUniform1iv(tmp, count, array));
}

void shader_create(shader *input, const char *vertex_path, const char *fragment_path){
    input->vertex_source = shader_load_source(vertex_path);
    input->fragment_source = shader_load_source(fragment_path);
    shader_create_program(input);

}

void shader_delete(shader *input){
    GLCall(glDeleteProgram(input->program));
}


//////////////////////////
//////BUFFERS////////////
/////////////////////////
void buffer_create(buffer *input, GLenum type, void *data, size_t length){
    GLCall(glGenBuffers(1, &(input->id)));
    
    if (data != NULL){
        input->data = malloc(length);
        memcpy(input->data, data, length);
    }
    

    input->type = type;
    input->length = length;
    buffer_bind(input);
    GLCall(glBufferData(input->type, input->length, input->data, GL_DYNAMIC_DRAW));
    buffer_unbind(input);
    

}

void buffer_update(buffer *input){
    buffer_bind(input);
    GLCall(glBufferSubData(input->type, 0, input->length, input->data));
    buffer_unbind(input);
}

void buffer_set_data(buffer *input, void *data, size_t length){
    buffer_bind(input);
    if (input->data != NULL){
        free(input->data);
    }
    input->data = malloc(length);
    memcpy(input->data, data, length);
    input->length = length;
    buffer_update(input);
}

void buffer_update_text_slot(buffer *input, float slot){
    buffer_bind(input);
    vertex *vertices = (vertex*) input->data;
    
    int vertices_count = input->length/sizeof(vertex);

    for (int i = 0; i < vertices_count; i++){
        vertices[i].text_slot = slot;
    }
    
    GLCall(glBufferSubData(input->type, 0, input->length, vertices));
    buffer_unbind(input);

}

void buffer_bind(buffer *input){
    GLCall(glBindBuffer(input->type, input->id));
}
void buffer_unbind(buffer *input){
    GLCall(glBindBuffer(input->type, 0));
}

void buffer_delete(buffer *input){
    glDeleteBuffers(1, &(input->id));
    free(input->data);
}




//////////////////////////
//////VERTEX ARRAYS////////////
/////////////////////////


void vertex_array_create(vertex_array *input){
    GLCall(glGenVertexArrays(1, &(input->id)));
    input->attribute_count = 0;
}

void vertex_array_bind(vertex_array *input){
    GLCall(glBindVertexArray(input->id));
}
void vertex_array_unbind(){
    GLCall(glBindVertexArray(0));
}

void vertex_array_delete(vertex_array *input){
    GLCall(glDeleteVertexArrays(1, &(input->id)));
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


//////////////////////////
//////RENDERABLE OBJECTS////////////
/////////////////////////
void renderable_object_link(renderable_object *input, vertex_array *vao, buffer *vbo, buffer *ibo, shader *shader){
    input->vao = vao;
    input->vbo = vbo;
    input->ibo = ibo;
    input->shader = shader;
    //input->texture = texture;
    vertex_array_bind(vao);
    buffer_bind(vbo);
    buffer_bind(ibo);
    for (int i = 0; i < vao->attribute_count; i++){
        GLCall(glVertexAttribPointer(i, vao->attributes[i].size, vao->attributes[i].type, vao->attributes[i].normalized, vao->attributes[i].stride, vao->attributes[i].pointer));
        GLCall(glEnableVertexAttribArray(i));
    }

    vertex_array_unbind(vao);
    buffer_unbind(vbo);
    buffer_unbind(ibo);
    

}

void renderable_object_create(renderable_object *input, void *vertices, int vertices_count, GLuint indices[], int indices_count, vertex_attrib_pointer attributes[], int attribute_count, shader *shader, texture *texture){
    input->shader = shader;
    if (texture != NULL){
        input->texture = texture;
    } else{
        input->texture = NULL;
    }
    vertex_array *VAO = calloc(1, sizeof(vertex_array));
    buffer *VBO = calloc(1, sizeof(buffer));
    buffer *IBO = calloc(1, sizeof(buffer));
    vertex_array_create(VAO);
    buffer_create(VBO, GL_ARRAY_BUFFER, vertices, vertices_count * sizeof(vertex));
    buffer_create(IBO, GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(GLuint) * indices_count);
    //IBO.indices_count = indices_count;

    vertex_array_bind(VAO);
    buffer_bind(VBO);
    buffer_bind(IBO);
    
    VAO->attribute_count = attribute_count;
    for (int i=0; i < attribute_count; i++){
        VAO->attributes[i] = attributes[i];
        GLCall(glVertexAttribPointer(attributes[i].index, attributes[i].size, attributes[i].type, attributes[i].normalized, attributes[i].stride, attributes[i].pointer));
        GLCall(glEnableVertexAttribArray(i));
    }
    

    vertex_array_unbind(VAO);
    buffer_unbind(VBO);
    buffer_unbind(IBO);
    input->vao = VAO;
    input->vbo = VBO;
    input->ibo = IBO;
}

void renderable_object_print(renderable_object *input, const char* name){
    printf("RENDERABLE_OBJECT: %s\n", name);
    printf("vertex_data:\n");
    vertex *vertex_data = (vertex*) input->vbo->data;
    int vertices_count = input->vbo->length/sizeof(vertex);
    vertices_print(vertex_data, vertices_count);

    GLuint *index_data = (GLuint*) input->ibo->data;
    int indices_count = input->ibo->length/sizeof(GLuint);
    printf("index data:");
    indices_print(input->ibo->data, indices_count);
    
}

void renderable_object_draw(renderable_object *input){
    glUseProgram(input->shader->program);
    vertex_array_bind((input->vao));
    if (input->texture != NULL){
        glBindTextureUnit(1, input->texture->id);// assign texture to slot 1
        buffer_update_text_slot((input->vbo), 1.0f);
    } else{
        buffer_update_text_slot((input->vbo), 0.0f);
    }
    int indices_count = input->ibo->length/sizeof(GLuint);
    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    vertex_array_unbind((input->vao));
}

void renderable_object_delete(renderable_object *input){
    vertex_array_delete((input->vao));
    buffer_delete((input->ibo));
    buffer_delete((input->vbo));
    shader_delete(input->shader);
}



//////////////////////////
//////Renderer////////////
/////////////////////////


void renderer_attach_object(renderer *input, renderable_object *object){
    input->object_count += 1;

    renderable_object *old_data = input->objects;
    renderable_object *new_data  = calloc(input->object_count, sizeof(renderable_object));

    if (input->object_count > 1){
        memcpy(new_data, old_data, sizeof(renderable_object) * input->object_count - 1);
        new_data[input->object_count-1] = *object;
    } else{
        new_data[0] = *object;
    }

    input->objects = new_data;


    if (old_data != NULL){
        free(old_data);
    }
    
}

void renderer_update_data(renderer *input){
    if (input->object_count == 0){
        printf("No renderable objects attached to renderer\n");
        return;
    }
    vertex_array_bind(input->vao);

    int vertex_data_length = 0;
    int index_data_length = 0;

    for (int i = 0; i < input->object_count; i++){
        vertex_data_length += input->objects[i].vbo->length;
        index_data_length += input->objects[i].ibo->length;

        if (input->objects[i].texture != NULL){
            int slot = i+1;
            glBindTextureUnit(slot, input->objects[i].texture->id);
            buffer_update_text_slot((input->objects[i].vbo), slot);
        } else{
            buffer_update_text_slot((input->objects[i].vbo), 0);
        }
    }


    void *vertex_data = malloc(vertex_data_length);
    void *index_data = malloc(index_data_length);
    if (vertex_data == NULL || index_data == NULL) {
        printf("falled to allocate vert or index data\n");
    }
    
    //populate the vertex and index data from every renderable object in the renderer
    int vertex_offset = 0;
    int index_offset = 0;

    for (int i = 0; i < input->object_count; i++){
        GLuint *tmp_indices = malloc(input->objects[i].ibo->length);
        memcpy(tmp_indices, input->objects[i].ibo->data, input->objects[i].ibo->length);

        for (unsigned int j = 0; j < (input->objects[i].ibo->length / sizeof(GLuint)); j++){
            tmp_indices[j] += 4*i;
        }

        memcpy((char *)vertex_data + vertex_offset, input->objects[i].vbo->data, input->objects[i].vbo->length);
        memcpy((char *)index_data + index_offset, tmp_indices, input->objects[i].ibo->length);
        vertex_offset += input->objects[i].vbo->length;
        index_offset += input->objects[i].ibo->length;


        free(tmp_indices);
    }

    buffer_set_data(input->vbo, vertex_data, vertex_data_length);
    buffer_set_data(input->ibo, index_data, index_data_length);


    free(vertex_data);
    free(index_data);

}

void renderer_initialize(renderer *input){

    vertex_array *VAO = calloc(1, sizeof(vertex_array));
    buffer *VBO = calloc(1, sizeof(buffer));
    buffer *IBO = calloc(1, sizeof(buffer));
    input->vao = VAO;
    input->vbo = VBO;
    input->ibo = IBO;
    vertex_array_create((input->vao));
    
    
    int vertex_data_length = 0;
    int index_data_length = 0;
    for (int i = 0; i < input->object_count; i++){
        vertex_data_length += input->objects[i].vbo->length;
        index_data_length += input->objects[i].ibo->length;
    }

    vertex_array_bind((input->vao));
    buffer_create((input->vbo), GL_ARRAY_BUFFER, NULL, vertex_data_length);
    buffer_create((input->ibo), GL_ELEMENT_ARRAY_BUFFER, NULL, index_data_length);

    
    buffer_bind((input->vbo));
    buffer_bind((input->ibo));

    vertex_attrib_pointer *tmp_attributes = &(input->objects[0].vao->attributes);
    int tmp_attribute_count = input->objects[0].vao->attribute_count;
    
    for (int i=0; i < tmp_attribute_count; i++){
        GLCall(glVertexAttribPointer(tmp_attributes[i].index, tmp_attributes[i].size, tmp_attributes[i].type, tmp_attributes[i].normalized, tmp_attributes[i].stride, tmp_attributes[i].pointer));
        GLCall(glEnableVertexAttribArray(i));
    }

}



// void renderer_initialize(renderer *input){
//     if (input->object_count == 0){
//         printf("No renderable objects attached to renderer\n");
//         return;
//     }
//     vertex_array *VAO = malloc(sizeof(vertex_array));
//     buffer *VBO = malloc(sizeof(buffer));
//     buffer *IBO = malloc(sizeof(buffer));
//     input->vao = VAO;
//     input->vbo = VBO;
//     input->ibo = IBO;

//     vertex_array_create((input->vao));
//     vertex_array_bind((input->vao));

//     //get sizee of all ther vertex and index data, also update the buffer texture slots
//     int vertex_data_length = 0;
//     int index_data_length = 0;

//     for (int i = 0; i < input->object_count; i++){
//         vertex_data_length += input->objects[i].vbo->length;
//         index_data_length += input->objects[i].ibo->length;

//         if (input->objects[i].texture != NULL){
//             int slot = i+1;
//             glBindTextureUnit(slot, input->objects[i].texture->id);
//             buffer_update_text_slot((input->objects[i].vbo), slot);
//         } else{
//             buffer_update_text_slot((input->objects[i].vbo), 0);
//         }

//     }

//     // use that size to malloc enough space to hold them
//     void *vertex_data = malloc(vertex_data_length);
//     void *index_data = malloc(index_data_length);
//     if (vertex_data == NULL || index_data == NULL) {
//         printf("falled to allocate vert or index data\n");
//     }
    
//     //populate the vertex and index data from every renderable object in the renderer
//     int vertex_offset = 0;
//     int index_offset = 0;
//     for (int i = 0; i < input->object_count; i++){

//         GLuint *tmp_indices = malloc(input->objects[i].ibo->length);
//         memcpy(tmp_indices, input->objects[i].ibo->data, input->objects[i].ibo->length);

//         for (unsigned int j = 0; j < (input->objects[i].ibo->length / sizeof(GLuint)); j++){
//             tmp_indices[j] += 4*i;
//         }

//         memcpy((char *)vertex_data + vertex_offset, input->objects[i].vbo->data, input->objects[i].vbo->length);
//         memcpy((char *)index_data + index_offset, tmp_indices, input->objects[i].ibo->length);
//         vertex_offset += input->objects[i].vbo->length;
//         index_offset += input->objects[i].ibo->length;

//         free(tmp_indices);
//     }

//     // we now have all the data in vertex_data and index_data, creat a buffer and put it in the GPU
//     vertex_array_bind((input->vao));
//     buffer_create((input->vbo), GL_ARRAY_BUFFER, vertex_data, vertex_data_length);
//     buffer_create((input->ibo), GL_ELEMENT_ARRAY_BUFFER, index_data, index_data_length);
//     //input->ibo.indices_count = input->indices_count;

    
//     buffer_bind((input->vbo));
//     buffer_bind((input->ibo));
//     //set attributes from just the first objects
//     vertex_attrib_pointer *tmp_attributes = &(input->objects[0].vao->attributes);
//     int tmp_attribute_count = input->objects[0].vao->attribute_count;
    
//     for (int i=0; i < tmp_attribute_count; i++){
//         GLCall(glVertexAttribPointer(tmp_attributes[i].index, tmp_attributes[i].size, tmp_attributes[i].type, tmp_attributes[i].normalized, tmp_attributes[i].stride, tmp_attributes[i].pointer));
//         GLCall(glEnableVertexAttribArray(i));
//     }

//     free(vertex_data);
//     free(index_data);
// }

void renderer_draw(renderer *input){   
    vertex_array_bind((input->vao));
    buffer_bind(input->vbo);
    buffer_bind(input->ibo);
    glUseProgram(input->objects[0].shader->program);
    GLCall(glDrawElements(GL_TRIANGLES, input->ibo->length/sizeof(GLuint), GL_UNSIGNED_INT, 0));
    vertex_array_unbind((input->vao));
}




//////////////////////////
//////Textures////////////
/////////////////////////

void texture_load(texture *input, const char *path){


    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &(input->id)));
    GLCall(glBindTexture(GL_TEXTURE_2D, input->id));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));    
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    int nrChannels;
    unsigned char *data = stbi_load(path, &(input->width), &(input->height), &nrChannels, 0);
    if (data){
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, input->width, input->height, 0, format, GL_UNSIGNED_BYTE, data));
        //glGenerateMipmap(GL_TEXTURE_2D);
        printf("Texture loaded successfully: %d x %d, channels: %d\n", input->width, input->height, nrChannels);
        input->data = data;

    }else{
        printf("Failed to load texture\n");
    }
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


void texture_delete(texture *input){
    GLCall(glDeleteTextures(1, &(input->id)));
}

void texture_bind(texture *input, int slot){
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, input->id));
}
void texture_unbind(){
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


//////////////////////////
//////Colors////////////
/////////////////////////

void color_set(color* dest, float r, float g, float b, float a){
    dest->r = r;
    dest->g = g;
    dest->b = b;
    dest->a = a;
}

//////////////////////////
//////Vertices/Indices setup////////////
/////////////////////////

void indices_print(void *data, int indices_count){
    GLuint *indices = (GLuint*) data;
    printf("indices: \n");
    for (int i = 0; i < indices_count; i++){
        printf("%u, ", indices[i]);
    }
    printf("\n");
}

void vertex_set_texture_slot(vertex *vertex, int slot){
    vertex->text_slot = slot;
}

void vertices_print(vertex *input, int count){
    for (int i = 0; i < count; i++){
        printf("===Vertex %d===\n", i);
        vertex_print(&(input[i]));
    }
}

void vertex_print(vertex *input){;
    printf("Position: %f, %f\n", input->position[0], input->position[1]);
    printf("texcoord: %f, %f\n", input->text_coords[0], input->text_coords[1]);
    printf("texindex: %f\n", input->text_slot);
    printf("color: %f, %f, %f, %f\n", input->color[0], input->color[1], input->color[2], input->color[3]);
    printf("\n");
}


void quad_set_texture_slot(quad *quad, float slot){
    quad->v0.text_slot = slot;
    quad->v1.text_slot = slot;
    quad->v2.text_slot = slot;
    quad->v3.text_slot = slot;
}

void quad_create(quad *dest, float x, float y, int size, color color, float texture_slot){
    struct vertex v0;
    v0.position[0] = x;
    v0.position[1] = y;
    v0.text_coords[0] = 0.0f;
    v0.text_coords[1] = 0.0f;
    v0.text_slot = texture_slot;
    v0.color[0] = color.r;
    v0.color[1] = color.g;
    v0.color[2] = color.b;
    v0.color[3] = color.a;
    
    struct vertex v1;
    v1.position[0] = x + size;
    v1.position[1] = y;
    v1.text_coords[0] = 1.0f;
    v1.text_coords[1] = 0.0f;
    v1.text_slot = texture_slot;
    v1.color[0] = color.r;
    v1.color[1] = color.g;
    v1.color[2] = color.b;
    v1.color[3] = color.a;

    struct vertex v2;
    v2.position[0] = x + size;
    v2.position[1] = y + size;
    v2.text_coords[0] = 1.0f;
    v2.text_coords[1] = 1.0f;
    v2.text_slot = texture_slot;
    v2.color[0] = color.r;
    v2.color[1] = color.g;
    v2.color[2] = color.b;
    v2.color[3] = color.a;

    struct vertex v3;
    v3.position[0] = x;
    v3.position[1] = y + size;
    v3.text_coords[0] = 0.0f;
    v3.text_coords[1] = 1.0f;
    v3.text_slot = texture_slot;
    v3.color[0] = color.r;
    v3.color[1] = color.g;
    v3.color[2] = color.b;
    v3.color[3] = color.a;

    dest->v0 = v0; 
    dest->v1 = v1;
    dest->v2 = v2;
    dest->v3 = v3;

}

//////////////////////////
//////GENERAL////////////
/////////////////////////
GLFWwindow* setup_opengl(int resolution_x, int resolution_y, void (*key_callback)(GLFWwindow*, int, int, int, int) ){
    GLFWwindow* window;
    if (!glfwInit()){exit(-1);}

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


        window = glfwCreateWindow(resolution_x, resolution_y, "OpenGL_Renderer", NULL, NULL);
        if (!window){
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(window);
        glewExperimental = GL_TRUE;
        // Query and print the actual OpenGL version being used
        const GLubyte* version = glGetString(GL_VERSION);
        printf("OpenGL version: %s\n", version);


        

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


        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_callback, NULL);

        
        return window;
}

//////////////////////////
//////Error Handling////////////
/////////////////////////

void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar* message, const void* userParam) {
    printf("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
           type, severity, message);
}

void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
};


bool GLLogCall(const char* function, const char* file, int line){
    GLenum error;
    while((error = glGetError())){
        
        printf("[OpenGL Error] (%d): %s, file: %s, line: %d\n", error, function, file, line);
        return false;
    }
    return true;
}

// void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
//                                 GLsizei length, const GLchar *message, const void *userParam) {
//     fprintf(stderr, "GL DEBUG MESSAGE: %s\n", message);
// }
