#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform vec4 color;
uniform bool colorOnly;

void main()
{
    if(colorOnly) 
    {
        FragColor = color;
        return;
    }

    vec4 texture_color = texture(texture1, TexCoords);

    if (texture_color.w > 0.0f)
    {
        texture_color = color;
    }

    FragColor = texture_color;
}