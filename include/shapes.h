#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h>

unsigned int squareVAO;

void LoadShapes()
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

    squareVAO = VAO;
}

#endif /* SHAPES_H */