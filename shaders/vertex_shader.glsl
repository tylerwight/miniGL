#version 330 core
layout(location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 uniformMVP;

void main()
{
    gl_Position = vec4(uniformMVP*aPos);
    TexCoord = aTexCoord;
}
