#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float u, v;
} TextureCoord;

typedef struct {
    unsigned int vertex, texture, normal;
} FacePoint;

typedef FacePoint FaceVertex[3];

typedef struct {
    Vertex *vertices;
    TextureCoord *texCoords;
    FaceVertex *faces;
    int numVertices, numTexCoords, numFaces;
} ObjModel;

void printObjModel(const ObjModel *model) {

    int i;
    printf("Number of Vertices: %d\n", model->numVertices);
    for (i = 0; i < model->numVertices; i++) {
        printf("Vertex %d: %f %f %f\n", i + 1, model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
    }

    printf("\nNumber of Texture Coordinates: %d\n", model->numTexCoords);
    for (i = 0; i < model->numTexCoords; i++) {
        printf("Texture Coord %d: %f %f\n", i + 1, model->texCoords[i].u, model->texCoords[i].v);
    }

    printf("\nNumber of Faces: %d\n", model->numFaces);
    for (i = 0; i < model->numFaces; i++) {
        printf("Face %d: %d/%d %d/%d %d/%d\n", i + 1, model->faces[i][0].vertex, model->faces[i][0].texture, model->faces[i][1].vertex, model->faces[i][1].texture, model->faces[i][2].vertex, model->faces[i][2].texture);
    }
}

unsigned int load_wavefront(const char* filename)
{
    ObjModel model;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numFaceProperties = 0;

    char line[256];
    int numVertices = 0, numTexCoords = 0, numFaces = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            numVertices++;
        } else if (line[0] == 'v' && line[1] == 't') {
            numTexCoords++;
        } else if (line[0] == 'f') {
            numFaces++;

            if(!numFaceProperties) {
                int v;

                int vertex = sscanf(line, "f %d %d %d", &v, &v, &v);
                int vertex_texture = sscanf(line, "f %d/%d %d/%d %d/%d", &v, &v, &v, &v, &v, &v);
                int vertex_texture_normal = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v, &v, &v, &v, &v, &v, &v, &v, &v);

                if (vertex == 3) {
                    numFaceProperties = 1;
                } else if (vertex_texture == 6) {
                    numFaceProperties = 2;
                } else if (vertex_texture_normal == 12) {
                    numFaceProperties = 3;
                }
            }
        }
    }

    model.vertices = (Vertex *)malloc(numVertices * sizeof(Vertex));
    model.texCoords = (TextureCoord *)malloc(numTexCoords * sizeof(TextureCoord));
    model.faces = (FaceVertex *)malloc(numFaces * sizeof(FaceVertex));
    model.numVertices = numVertices;
    model.numTexCoords = numTexCoords;
    model.numFaces = numFaces;

    fseek(file, 0, SEEK_SET);

    int vertexIndex = 0, texCoordIndex = 0, faceIndex = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &model.vertices[vertexIndex].x, &model.vertices[vertexIndex].y, &model.vertices[vertexIndex].z);
            vertexIndex++;
        } else if (line[0] == 'v' && line[1] == 't') {
            sscanf(line, "vt %f %f", &model.texCoords[texCoordIndex].u, &model.texCoords[texCoordIndex].v);
            texCoordIndex++;
        } else if (line[0] == 'f') {

            if(numFaceProperties == 1) {
                sscanf(line, "f %d %d %d",
                       &model.faces[faceIndex][0].vertex,
                       &model.faces[faceIndex][1].vertex,
                       &model.faces[faceIndex][2].vertex);

                       model.faces[faceIndex][0].texture = -1;
            }
            else if (numFaceProperties == 2)
            {
                sscanf(line, "f %d/%d %d/%d %d/%d",
                       &model.faces[faceIndex][0].vertex, &model.faces[faceIndex][0].texture,
                       &model.faces[faceIndex][1].vertex, &model.faces[faceIndex][1].texture,
                       &model.faces[faceIndex][2].vertex, &model.faces[faceIndex][2].texture);
            }
            else if (numFaceProperties == 3)
            {
                sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                       &model.faces[faceIndex][0].vertex, &model.faces[faceIndex][0].texture, &model.faces[faceIndex][0].normal,
                       &model.faces[faceIndex][1].vertex, &model.faces[faceIndex][1].texture, &model.faces[faceIndex][1].normal,
                       &model.faces[faceIndex][2].vertex, &model.faces[faceIndex][2].texture, &model.faces[faceIndex][2].normal);
                
            }
            faceIndex++;
        }
    }

    printObjModel(&model);

    fclose(file);

    int numIndices = model.numFaces * 3;
    unsigned int* indices = (unsigned int*)malloc(numIndices * sizeof(unsigned int));

    int i;
    for (i = 0; i < model.numFaces; ++i)
    {
        indices[i * 3] = model.faces[i][0].vertex - 1;
        indices[i * 3 + 1] = model.faces[i][1].vertex - 1;
        indices[i * 3 + 2] = model.faces[i][2].vertex - 1;
    }

    for (i = 0; i < numIndices; ++i)
    {
        printf("indice[%d]:%d\n", i, indices[i]);
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
     
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, model.numVertices * sizeof(Vertex), model.vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint texCoordVBO;
    glGenBuffers(1, &texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, model.numTexCoords * sizeof(TextureCoord), model.texCoords, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
    glEnableVertexAttribArray(1);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vao;
}

#endif /* WAVEFRONT_H */