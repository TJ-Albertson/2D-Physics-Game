#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

#include <camera.h>
#include <physics.h>

typedef enum Input_Type {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    JUMP,
    SPRINT,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
    CAMERA_DOWN
} Input_Type;

void ProcessKeyboard(Camera *camera, Input_Type input, Vector2D* velocity)
{
    switch (camera->Type)
    {
    case THIRD:
    {
        switch (input)
        {
        case LEFT:
            velocity->x += 0.1f * dt;
            break;

        case RIGHT:
            velocity->x -= 0.1f * dt;
            break;

        case JUMP: {
            velocity->y = velocity->y + 0.1f * dt;
        }
            
            break;

        default:
            break;
        }
    }
    break;

    case FREE:
    {
        switch (input)
        {
        case LEFT:
            camera->Position.x += 0.1f;
            break;

        case RIGHT:
            camera->Position.x -= 0.1f;
            break;

        case UP:
            camera->Position.y += 0.1f;
            break;

        case DOWN:
            camera->Position.y -= 0.1f;
            break;

        default:
            break;
        }
    }
    break;

    default:
        break;
    }
}

void ProcessInput(GLFWwindow* window, Camera* camera, Vector2D* velocity)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

   
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ProcessKeyboard(camera, UP, velocity);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ProcessKeyboard(camera, DOWN, velocity);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ProcessKeyboard(camera, LEFT, velocity);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ProcessKeyboard(camera, RIGHT, velocity);

    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        ProcessKeyboard(camera, JUMP, velocity);

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
        camera->Position.z -= (float)yoffset * 0.5f;

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