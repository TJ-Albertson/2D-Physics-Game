#version 330 core
out vec4 FragColor;

uniform vec2 offset;
uniform vec3 color;

uniform float gridSize;
uniform float lineThickness;

in vec2 FragCoord; 

void main() {

    vec4 uniformColor = vec4(offset, gridSize, lineThickness);

    bool isGrid = mod((FragCoord + offset).x * gridSize, 1.0) < lineThickness || mod((FragCoord + offset).y * gridSize, 1.0) < lineThickness;
    vec3 color = isGrid ? vec3(0.25f, 0.25f, 0.25f) : vec3(1.0, 1.0, 1.0);

    FragColor = vec4(color, 0.35f);

    if (FragCoord.x < -10) {
        FragColor = uniformColor;
    }
}
