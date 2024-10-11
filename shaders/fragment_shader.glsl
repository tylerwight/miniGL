#version 330 core
in vec2 v_tex_coord;
in float v_tex_id;
in vec4 v_color;

out vec4 FragColor;


uniform sampler2D uniformTexture;
uniform sampler2D u_textures[2];
uniform vec3 uniformColor;

void main(){
    int index = int(v_tex_id);
    //FragColor = v_color;
    FragColor = texture(u_textures[0], v_tex_coord);
}
