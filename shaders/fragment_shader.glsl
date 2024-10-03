#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 uniformColor;

void main()
{
    //FragColor = vec4(uniformColor, 1.0);
    FragColor = texture(texture1, TexCoord);
}
