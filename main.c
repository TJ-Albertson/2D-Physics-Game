#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <my_math/matrix.h>

#include <input.h>
#include <camera.h>
#include <shader.h>
#include <sprite.h>
#include <wavefront.h>
#include <platform.h>
#include <grid.h>
#include <shapes.h>
#include <physics.h>    

unsigned int SCR_WIDTH = 2000;
unsigned int SCR_HEIGHT = 1200;
const float RENDER_DISTANCE = 1000.0f;

GLFWwindow* initialize_window();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera playerCamera;

int main() {

    GLFWwindow* glfw_window = initialize_window();
    playerCamera = CreateCamera();

    LoadShapes();

    ShaderID basicShader = createShader("resources/shaders/basic.vs", "resources/shaders/basic.fs");
    ShaderID gridShader = createShader("resources/shaders/grid.vs", "resources/shaders/grid.fs");

    unsigned int circle_VAO = load_wavefront("resources/textures/circle.obj");
    unsigned int textureID = LoadTexture("resources/textures/blue_circle.png");

    Sprite sprite;
    InitSprite(&sprite, "resources/textures/circle.png");

    /*
    glLineWidth(3.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    */

    Platform platform;
    platform.width = 5.0f;
    platform.height = 2.0f;
    platform.position.x = 0.0f;
    platform.position.y = -3.0f;

    

    float currentTime = glfwGetTime();
    float accumulator = 0.0;

    state.position.x = 0.0f;
    state.position.y = 0.0f;
    state.velocity.x = 0.0f;
    state.velocity.y = 0.0f;

    State previousState;
    State currentState = state;
    double prevTime = glfwGetTime();

    
    glUseProgram(basicShader);
    glUniform1i(glGetUniformLocation(basicShader, "texture1"), 0);

    while (!glfwWindowShouldClose(glfw_window)) 
    {
        float newTime = glfwGetTime();

        float deltaTime = newTime - currentTime;

        float frameTime = newTime - currentTime;
        if (frameTime > 0.25)
            frameTime = 0.25;
        float currentTime = newTime;

        float devTimeMultiplier = 1.0f;

        accumulator += frameTime;

        
        while (accumulator >= dt) {
            previousState = currentState;
            IntegrateState(&currentState, t, dt * devTimeMultiplier);
            t += dt;
            accumulator -= dt;
        }

        const float alpha = accumulator / dt;

        /* interpolating between pevious and current state */
        state.velocity.x = currentState.velocity.x * alpha + previousState.velocity.x * (1.0f - alpha);
        state.velocity.y = currentState.velocity.y * alpha + previousState.velocity.y * (1.0f - alpha);

        state.position.x = currentState.position.x * alpha + previousState.position.x * (1.0f - alpha);
        state.position.y = currentState.position.y * alpha + previousState.position.y * (1.0f - alpha);

        printf("state.position.y: %f\n", state.position.y);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Mat4* perspect = perspective(degreesToRadians(playerCamera.FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, RENDER_DISTANCE);
        Mat4* orthographic = ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

        playerCamera.Position.x = state.position.x;
        playerCamera.Position.y = state.position.y;


        Mat4* view = GetViewMatrix(playerCamera);

        
        DrawGrid(gridShader, squareVAO, perspect, view, playerCamera);
        

        glUseProgram(basicShader);
        setShaderMat4(basicShader, "projection", perspect);
        setShaderMat4(basicShader, "view", view);

       
        DrawPlatform(basicShader, squareVAO, platform);
        

        Mat4 model;
        clear_matrix(&model);
        translateMat4(&model, state.position.x, state.position.y, 0.0f);
        setShaderMat4(basicShader, "model", &model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindVertexArray(circle_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /* End */
        glfwSwapBuffers(glfw_window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}







GLFWwindow* initialize_window()
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* glfw_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Physics Game", NULL, NULL);
    if (!glfw_window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(glfw_window);
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(glfw_window, mouse_callback);
    glfwSetScrollCallback(glfw_window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
    }
    
    /* OpenGL Global State */
    /*  glEnable(GL_DEPTH_TEST); */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return glfw_window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    SCR_HEIGHT = height;
    SCR_WIDTH = width;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ProcessMouseScroll(&playerCamera, yoffset);
}