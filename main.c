#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <my_math/matrix.h>

#include <input.h>
#include <camera.h>
#include <shader.h>
#include <sprite.h>
#include <wavefront.h>

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

    /* Grid */
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
    /* Grid */
    
    glUseProgram(basicShader);
    glUniform1i(glGetUniformLocation(basicShader, "texture1"), 0);

    while (!glfwWindowShouldClose(glfw_window)) 
    {
        ProcessInput(glfw_window, &playerCamera);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        float n = 0.1f;
        float fov = playerCamera.FOV;
        float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;

        float r = n * tan(0.5f * (fov * PI / 180.0f));
        float t = r * aspectRatio;
            
        Mat4* grid_projection = generateClipMatrix(n, r, t);
        
        Mat4* perspect = perspective(degreesToRadians(playerCamera.FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, RENDER_DISTANCE);
        
        Mat4* orthographic = ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
        
        Mat4* view = GetViewMatrix(playerCamera);

        /* Grid */
        glUseProgram(gridShader);
        setShaderMat4(gridShader, "projection", perspect);
        setShaderMat4(gridShader, "view", view);

        Mat4 grid_model;
        clear_matrix(&grid_model);

        setShaderVec2(gridShader, "offset", playerCamera.Position.x * 0.1f, playerCamera.Position.y * 0.1f);

        translateMat4(&grid_model, playerCamera.Position.x, playerCamera.Position.y, 0.0f);
        scaleMat4(&grid_model, SCR_WIDTH / 100, SCR_HEIGHT / 100, 1.0f);
        setShaderMat4(basicShader, "model", &grid_model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        /* Grid */


        glUseProgram(basicShader);

        setShaderMat4(basicShader, "projection", perspect);
        setShaderMat4(basicShader, "view", view);

        Mat4 model;
        clear_matrix(&model);
        setShaderMat4(basicShader, "model", &model);

        glBindVertexArray(circle_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        translateMat4(&model, playerCamera.Position.x, playerCamera.Position.y, 0.0f);
        setShaderMat4(basicShader, "model", &model);

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