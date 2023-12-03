#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>

#include <shader.h>
#include <camera.h>
#include <my_math/matrix.h>

void DrawGrid(unsigned int ShaderID, unsigned int VAO, Mat4 *perspect, Mat4 *view, Camera playerCamera)
{
    glUseProgram(ShaderID);

    setShaderMat4(ShaderID, "projection", perspect);
    setShaderMat4(ShaderID, "view", view);

    float grid_scale = 125;
    float grid_move_speed = 1.0f / grid_scale;

    Mat4 grid_model;
    clear_matrix(&grid_model);
    translateMat4(&grid_model, playerCamera.Position.x, playerCamera.Position.y, 0.0f);
    scaleMat4(&grid_model, grid_scale, grid_scale, 1.0f);
    setShaderMat4(ShaderID, "model", &grid_model);
    setShaderVec2(ShaderID, "offset", playerCamera.Position.x * grid_move_speed, playerCamera.Position.y * grid_move_speed);

    setShaderVec4(ShaderID, "color", 0.35f, 0.35f, 0.35f, 0.25f);
    glBindVertexArray(VAO);

    /* Medium */
    setShaderFloat(ShaderID, "gridSize", 125.0);

    float medium_thickness = 0.004f * playerCamera.Position.z;
        
    setShaderFloat(ShaderID, "lineThickness", medium_thickness);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /* Small */
    if (playerCamera.Position.z < 7.0f)
    {
        setShaderVec4(ShaderID, "color", 0.35f, 0.35f, 0.35f, 0.35f / playerCamera.Position.z );
        setShaderFloat(ShaderID, "gridSize", 1250.0);
        setShaderFloat(ShaderID, "lineThickness", 0.02f * playerCamera.Position.z);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    float color_line_thickness = 0.0005f * playerCamera.Position.z;

    printf("color_line_thickness: %f\n", color_line_thickness);

    /* Red */
    setShaderVec4(ShaderID, "color", 1.0f, 0.0f, 0.0f, 0.35f);
    setShaderFloat(ShaderID, "gridSize", 12.5);
    setShaderFloat(ShaderID, "lineThickness", color_line_thickness);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /* Blue */
    setShaderVec2(ShaderID, "offset", (playerCamera.Position.x + 25.0f) * grid_move_speed, (playerCamera.Position.y + 25.0f) * grid_move_speed);
    setShaderVec4(ShaderID, "color", 0.0f, 0.0f, 1.0f, 0.35f);
    setShaderFloat(ShaderID, "gridSize", 12.5);
    setShaderFloat(ShaderID, "lineThickness", color_line_thickness);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

#endif /* GRID_H */
