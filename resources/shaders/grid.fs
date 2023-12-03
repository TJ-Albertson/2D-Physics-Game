#version 330 core
out vec4 FragColor;

uniform vec2 offset;
uniform vec4 color;

uniform float gridSize;
uniform float lineThickness;

in vec2 FragCoord; 

void main() {

    vec4 uniformColor = vec4(offset, gridSize, lineThickness);

    bool isGrid = mod((FragCoord + offset).x * gridSize, 1.0) < lineThickness || mod((FragCoord + offset).y * gridSize, 1.0) < lineThickness;
    FragColor = isGrid ? color : vec4(1.0, 1.0, 1.0, 0.0);

   

    if (FragCoord.x < -10) {
        FragColor = uniformColor;
    }
}
