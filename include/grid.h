#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>

#include <shader.h>

typedef struct Grid {
    unsigned int VAO;
    unsigned int numIndices;
};

Grid CreateGrid() 
{
    float vertices[] = {
        -1.0f, -1.0f,  
         1.0f, -1.0f, 
         1.0f,  1.0f,  
        -1.0f,  1.0f  
    };

    unsigned int indices[] = {
        0, 1, 2,  
        0, 2, 3  
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
};

void DrawGrid(unsigned int ShaderID, Mat4* perspect, matView) {
    glUseProgram(gridShader);
        
    setShaderMat4(gridShader, "projection", perspect);
    setShaderMat4(gridShader, "view", view);

    float grid_scale = 25;
    float grid_move_speed = 1.0f / grid_scale;

    Mat4 grid_model;
    clear_matrix(&grid_model);
    translateMat4(&grid_model, playerCamera.Position.x, playerCamera.Position.y, 0.0f);
    scaleMat4(&grid_model, grid_scale, grid_scale, 1.0f);
    setShaderMat4(basicShader, "model", &grid_model);

    setShaderVec2(gridShader, "offset", playerCamera.Position.x * grid_move_speed, playerCamera.Position.y * grid_move_speed);
    setShaderFloat(gridShader, "gridSize", 150.0);
    setShaderFloat(gridShader, "lineThickness", 0.05f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);    
}

#endif /* GRID_H */

