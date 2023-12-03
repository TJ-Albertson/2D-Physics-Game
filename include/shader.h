#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <my_math/matrix.h>

typedef unsigned int ShaderID;

void checkCompileErrors(GLuint shader, const char* type);
ShaderID createShader(const char* vertexPath, const char* fragmentPath);

void checkGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        const char* errorString;
        switch (error) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                errorString = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            default:
                errorString = "Unknown error";
                break;
        }
        fprintf(stderr, "OpenGL Error: %s\n", errorString);
    }
}


void setShaderBool(unsigned int shaderID, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shaderID, name), (int)value);
}

void setShaderMat4(unsigned int shaderID, const char* name, const Mat4* mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, mat[0][0]);
}

void setShaderVec2(unsigned int shaderID, const char* name, float x, float y)
{

    int location = glGetUniformLocation(shaderID, name);

    if (location != -1) {
          glUniform2f(location, x, y);
      
    } else {
        /*printf("Error: Uniform %s not found in shader %d\n", name, shaderID);*/
    }
}

void setShaderVec4(unsigned int shaderID, const char* name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(shaderID, name), x, y, z, w);
}

void setShaderFloat(unsigned int shaderID, const char* name, float value)
{
    int location = glGetUniformLocation(shaderID, name);

    if (location != -1) {
        glUniform1f(location, value);
      
    } else {
       /* printf("Error: Uniform %s not found in shader %d\n", name, shaderID);*/
    }
}

ShaderID createShader(const char* vertexPath, const char* fragmentPath) 
{
    ShaderID shaderID;
    char* vertexCode;
    char* fragmentCode;
    FILE* vShaderFile;
    FILE* fShaderFile;
    long length;

    vShaderFile = fopen(vertexPath, "rb");
    if (vShaderFile)
    {
        fseek(vShaderFile, 0, SEEK_END);
        length = ftell(vShaderFile);
        fseek(vShaderFile, 0, SEEK_SET);

        vertexCode = (char*)malloc(length + 1);

        if (vertexCode)
        {
            fread(vertexCode, 1, length, vShaderFile);
            vertexCode[length] = '\0';
        }

        fclose(vShaderFile);
    }

    fShaderFile = fopen(fragmentPath, "rb");
    if (fShaderFile)
    {
        fseek(fShaderFile, 0, SEEK_END);
        length = ftell(fShaderFile);
        fseek(fShaderFile, 0, SEEK_SET);

        fragmentCode = (char*)malloc(length + 1);
        if (fragmentCode)
        {
            fread(fragmentCode, 1, length, fShaderFile);
            fragmentCode[length] = '\0';
        }
        fclose(fShaderFile);
    }

    const char* vShaderCode = vertexCode;
    const char* fShaderCode = fragmentCode;

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertex);
    glAttachShader(shaderID, fragment);
    glLinkProgram(shaderID);
    checkCompileErrors(shaderID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderID;
}

void checkCompileErrors(GLuint shader, const char* type) 
{
    GLint success;
    GLchar infoLog[1024];

    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n -- --------------------------------------------------- --\n", type, infoLog);
            exit(EXIT_FAILURE);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n -- --------------------------------------------------- --\n", type, infoLog);
            exit(EXIT_FAILURE);
        }
    }
}

#endif /* SHADER_H */