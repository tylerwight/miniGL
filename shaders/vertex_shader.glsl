#version 450 core
layout(location = 0) in vec4 a_pos;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in float a_tex_id;
layout(location = 3) in vec4 a_color;

out vec2 v_tex_coord;
out vec4 v_color;
out float v_tex_id;

uniform mat4 uniform_view;
uniform mat4 uniform_projection;
uniform mat4 uniform_model_matrix;

void main(){
    gl_Position = vec4((uniform_projection * uniform_view * uniform_model_matrix) * a_pos);
    v_tex_coord = a_tex_coord;
    v_tex_id = a_tex_id;
    v_color = a_color;

}
