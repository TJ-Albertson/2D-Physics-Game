#ifndef PLATFORM_H_H
#define PLATFORM_H

#include <glad/glad.h>

#include <my_math/vector.h> 
#include <my_math/matrix.h>
#include <shader.h>

typedef struct Platform {
    float width;
    float height;

    Vector2D position;

    float rotation;
} Platform;


void CreatePlatform()
{

}

void DrawPlatform(unsigned int shaderID, unsigned int VAO, Platform platform)
{
    glUseProgram(shaderID);

    Mat4 model;
    clear_matrix(&model);

    translateMat4(&model, platform.position.x, platform.position.y, 0.0f);
    scaleMat4(&model, platform.width, platform.height, 1.0f);
    setShaderMat4(shaderID, "model", &model);

    setShaderBool(shaderID, "colorOnly", 1);
    setShaderVec4(shaderID, "color", 0.45f, 0.45f, 0.45f, 0.45f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);   

    setShaderBool(shaderID, "colorOnly", 0);
}

#endif /* PLATFORM_H_H */