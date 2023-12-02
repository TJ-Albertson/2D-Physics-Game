#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 FragCoord;

void main() 
{
    FragCoord = vec2(aPos.x, aPos.y);
    gl_Position =  projection * view * model * vec4(aPos, 1.0f);
}
