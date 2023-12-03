#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>

#include <shader.h>
#include <camera.h>

void DrawGrid(unsigned int ShaderID, unsigned int VAO, Mat4* perspect, Mat4* view, Camera playerCamera) {
    glUseProgram(ShaderID);
        
    setShaderMat4(ShaderID, "projection", perspect);
    setShaderMat4(ShaderID, "view", view);

    float grid_scale = 150;
    float grid_move_speed = 1.0f / grid_scale;

    Mat4 grid_model;
    clear_matrix(&grid_model);
    translateMat4(&grid_model, playerCamera.Position.x, playerCamera.Position.y, 0.0f);
    scaleMat4(&grid_model, grid_scale, grid_scale, 1.0f);
    setShaderMat4(ShaderID, "model", &grid_model);

    setShaderVec2(ShaderID, "offset", playerCamera.Position.x * grid_move_speed, playerCamera.Position.y * grid_move_speed);
    setShaderFloat(ShaderID, "gridSize", 50.0);
    setShaderFloat(ShaderID, "lineThickness", 0.05f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    setShaderVec2(ShaderID, "offset", playerCamera.Position.x * grid_move_speed + 3, playerCamera.Position.y * grid_move_speed + 3);
    setShaderFloat(ShaderID, "gridSize", 10.0);
    setShaderFloat(ShaderID, "lineThickness", 0.01f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);   

}

#endif /* GRID_H */

