#version 330 core
out vec4 FragColor;

in vec2 FragCoord; // Input variable from vertex shader (normalized coordinates)
uniform vec2 offset;
uniform vec2 resolution;

void main() {
    // Define grid parameters
    float gridSize = 5.0;
    float lineThickness = 0.03; // Adjust this value to control line thickness

    vec2 uv = FragCoord.xy / resolution; // Normalize coordinates

    // Create grid pattern with thicker lines and offset
    bool isGrid = mod((uv + offset).x * gridSize, 1.0) < lineThickness || mod((uv + offset).y * gridSize, 1.0) < lineThickness;
    vec3 color = isGrid ? vec3(0.3, 0.3, 0.3) : vec3(1.0, 1.0, 1.0);

    // Output final color
    FragColor = vec4(color, 1.0);
}
