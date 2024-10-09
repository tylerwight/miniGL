#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uniformTexture;
uniform vec3 uniformColor;

void main()
{
    //FragColor = vec4(uniformColor, 1.0);
    FragColor = texture(uniformTexture, TexCoord);
}
