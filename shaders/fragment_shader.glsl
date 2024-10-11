#version 450 core
in vec2 v_tex_coord;
in float v_tex_id;
in vec4 v_color;

out vec4 FragColor;


uniform sampler2D u_textures[10];
uniform vec3 uniformColor;

void main(){
    int index = int(v_tex_id);

    if (index == 0){
        FragColor = v_color;
    } else{
        FragColor = texture(u_textures[index], v_tex_coord);
    }
    
    //FragColor = vec4(v_tex_id, v_tex_id, v_tex_id, 1.0);
}
