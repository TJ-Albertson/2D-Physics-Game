#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

#include <camera.h>
#include <physics.h>

typedef enum Input_Type {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP,
    SPRINT,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
    CAMERA_DOWN
} Input_Type;

void ProcessKeyboard(Camera* camera, Input_Type input)
{
    switch (camera->Type)
    {
    case THIRD: {
        switch (input)
        {
        case CAMERA_LEFT:
            state.velocity.x += 1.0f * dt;
            break;

        case CAMERA_RIGHT:
            state.velocity.x -= 1.0f * dt;
            break;

        case JUMP:
            state.velocity.y += 2.0f * dt;
            break;  

        default:
            break;
        }
    } break;

    case FREE: {
        switch (input)
        {
        case CAMERA_LEFT:
            camera->Position.x += 0.1f;
            break;

        case CAMERA_RIGHT:
            camera->Position.x -= 0.1f;
            break;

        case CAMERA_UP:
            camera->Position.y += 0.1f;
            break;   

        case CAMERA_DOWN:
            camera->Position.y -= 0.1f;
            break; 

        default:
            break;
        }
    } break;   
    
    default:
        break;
    }
    
}

void ProcessInput(GLFWwindow* window, Camera* camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

   
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ProcessKeyboard(camera, CAMERA_UP);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ProcessKeyboard(camera, CAMERA_DOWN);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ProcessKeyboard(camera, CAMERA_LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ProcessKeyboard(camera, CAMERA_RIGHT);

    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        ProcessKeyboard(camera, JUMP);

        /*
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        ProcessKeyboard(camera, SPRINT, velocity, dt);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        ProcessKeyboard(camera, CAMERA_LEFT, velocity, dt);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        ProcessKeyboard(camera, CAMERA_RIGHT, velocity, dt);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        ProcessKeyboard(camera, CAMERA_UP, velocity, dt);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        ProcessKeyboard(camera, CAMERA_DOWN, velocity, dt);
    */
}

void ProcessMouseScroll(Camera* camera, float yoffset)
{
    switch (camera->Projection)
    {
    case ORTHOGRAPHIC: {
        camera->Position.z -= (float)yoffset;

        if (camera->Position.z < 1.0f)
            camera->Position.z = 1.0f;
        if (camera->Position.z > 20.0f)
            camera->Position.z = 20.0f;   
    } break;

    case PERSPECTIVE:
        break;
    
    default:
        break;
    }

    
}

#endif /* INPUT_H */