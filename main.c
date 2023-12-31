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
#include <collision.h>  

unsigned int SCR_WIDTH = 2000;
unsigned int SCR_HEIGHT = 1200;
const float RENDER_DISTANCE = 1000.0f;

GLFWwindow* initialize_window();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera playerCamera;
GLFWwindow* glfw_window;

AABB aabb_platform;

void IntegrateState(State* state, float time, float dt)
{
    
    
    Vector2D acceleration = gravity;


    float friction_coefficient = 1.0f;
    state->velocity.x = state->velocity.x + (acceleration.x - friction_coefficient * state->velocity.x) * dt;
    state->velocity.y = state->velocity.y + (acceleration.y - friction_coefficient * state->velocity.y) * dt;

    CollisionDetection();
    CollisionResponse(); 
    if (state->velocity.x > EPSILON || state->velocity.y > EPSILON) {
        
    }
    /* PlayerCollision(state, aabb_platform); */
    
    
    /*Collision(velocity, state.position, dt); */
    state->position.x = state->position.x + state->velocity.x * dt;
    state->position.y = state->position.y + state->velocity.y * dt;
}

void printBinary(uint8_t value) {
    int i;
    for (i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}


int main() {

    glfw_window = initialize_window();
    playerCamera = CreateCamera();

    LoadShapes();

    ShaderID basicShader = createShader("resources/shaders/basic.vs", "resources/shaders/basic.fs");
    ShaderID gridShader = createShader("resources/shaders/grid.vs", "resources/shaders/grid.fs");

    unsigned int circle_VAO = load_wavefront("resources/textures/circle.obj");
    unsigned int textureID = LoadTexture("resources/textures/blue_circle.png");

    unsigned int circle_VAO2 = load_wavefront("resources/textures/circle2.obj");
    unsigned int textureID2 = LoadTexture("resources/textures/circle.png");

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
    platform.position.y = -2.0f;

    float halfWidth = (platform.width * 2) / 2.0f;
    float halfHeight = (platform.height * 2) / 2.0f;

    aabb_platform.min.x = platform.position.x - halfWidth;
    aabb_platform.min.y = platform.position.y - halfHeight;
    aabb_platform.max.x = platform.position.x + halfWidth;
    aabb_platform.max.y = platform.position.y + halfHeight;

    float currentTime = glfwGetTime();
    float accumulator = 0.0;

    state.position.x = 0.0f;
    state.position.y = 0.0f;
    state.velocity.x = 0.0f;
    state.velocity.y = 0.0f;

    /* new collision */
    initialize_collision();
    boxes[0] = aabb_platform;
    num_boxes++;

    DynamicObject dynamic_object;
    dynamic_object.state.position.x = 0.0f;
    dynamic_object.state.position.y = 2.0f;
    dynamic_object.state.velocity.x = 0.0f;
    dynamic_object.state.velocity.y = 0.0f;

    dynamic_object.currentState.position.x = 0.0f;
    dynamic_object.currentState.position.y = 2.0f;
    dynamic_object.currentState.velocity.x = 0.0f;
    dynamic_object.currentState.velocity.y = 0.0f;

    dynamic_object.previousState.position.x = 0.0f;
    dynamic_object.previousState.position.y = 2.0f;
    dynamic_object.previousState.velocity.x = 0.0f;
    dynamic_object.previousState.velocity.y = 0.0f;

    dynamic_object.type = DYNAMIC_SPHERE;
    dynamic_object.collider.sphere.center.x = 0.0f;
    dynamic_object.collider.sphere.center.y = 0.0f;
    dynamic_object.collider.sphere.radius = 0.5f;

    dynamic_object.flags = 0.0f;

    dynamic_objects[0] = dynamic_object;
    num_dynamic_objects++;

    /* dynamic_objects_generate(5); */
    /* new collision */


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

            int i;
            for(i = 0; i < num_dynamic_objects; ++i)
            {
                dynamic_objects[i].previousState = dynamic_objects[i].currentState;
                
                if (i == 0)
                {
                    ProcessInput(glfw_window, &playerCamera, &dynamic_objects[i].currentState.velocity);
                }
                
                IntegrateState(&dynamic_objects[i].currentState, t, dt * devTimeMultiplier);
            }
            
            t += dt;
            accumulator -= dt;
        }

        const float alpha = accumulator / dt;

        /* interpolating between previous and current state */
        int i;
        for(i = 0; i < num_dynamic_objects; ++i)
        {
            dynamic_objects[i].state.velocity.x = dynamic_objects[i].currentState.velocity.x * alpha + dynamic_objects[i].previousState.velocity.x * (1.0f - alpha);
            dynamic_objects[i].state.velocity.y = dynamic_objects[i].currentState.velocity.y * alpha + dynamic_objects[i].previousState.velocity.y * (1.0f - alpha);

            dynamic_objects[i].state.position.x = dynamic_objects[i].currentState.position.x * alpha + dynamic_objects[i].previousState.position.x * (1.0f - alpha);
            dynamic_objects[i].state.position.y = dynamic_objects[i].currentState.position.y * alpha + dynamic_objects[i].previousState.position.y * (1.0f - alpha);
        }

        

        /* printf("dynamic_object.state.position: x%f y%f\n", dynamic_object.state.position.x, dynamic_object.state.position.y); */

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Mat4* perspect = perspective(degreesToRadians(playerCamera.FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, RENDER_DISTANCE);
        Mat4* orthographic = ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

        
        playerCamera.Position.x = dynamic_objects[0].state.position.x;
        playerCamera.Position.y = dynamic_objects[0].state.position.y;


       

        Mat4* view = GetViewMatrix(playerCamera);

        
        DrawGrid(gridShader, squareVAO, perspect, view, playerCamera);
        

        glUseProgram(basicShader);
        setShaderMat4(basicShader, "projection", perspect);
        setShaderMat4(basicShader, "view", view);

       
        DrawPlatform(basicShader, squareVAO, platform);
        

        
       
        
        

        for (i = 0; i < 25; ++i)
        {
           
            setShaderVec4(basicShader, "color", 1.0f, 1.0f, 0.0f, 1.0f);
        
            Mat4 model;
            clear_matrix(&model);
            translateMat4(&model, collision_points[i].x, collision_points[i].y, 0.0f);
            scaleMat4(&model, 0.1f, 0.1f, 0.1f);
            setShaderMat4(basicShader, "model", &model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID2);

            glBindVertexArray(circle_VAO2);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

        }

        for (i = 0; i < num_dynamic_objects; ++i)
        {
            Mat4 model;
            clear_matrix(&model);
            translateMat4(&model, dynamic_objects[i].state.position.x, dynamic_objects[i].state.position.y, 0.0f);
            setShaderMat4(basicShader, "model", &model);

            setShaderVec4(basicShader, "color", 0.0f, 1.0f, 0.0f, 1.0f);

            /*
            printf("dynamic_objects[%d].flags: ", i);
            printBinary(dynamic_objects[i].flags);
            printf("\n");
            */
            

            uint8_t collision_flag = (dynamic_objects[i].flags >> 0) & 1;

            if(collision_flag)
            {
                setShaderVec4(basicShader, "color", 1.0f, 0.0f, 0.0f, 1.0f);
                
            }

            uint8_t dynamic_collision = (dynamic_objects[i].flags >> 1) & 1;

            if(dynamic_collision)
            {
                setShaderVec4(basicShader, "color", 0.0f, 0.0f, 1.0f, 1.0f);
            }
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glBindVertexArray(circle_VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

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
    glfwWindowHint(GLFW_SAMPLES, 8);

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
    glEnable(GL_MULTISAMPLE);  
    
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