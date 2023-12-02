#ifndef PHYSICS_H
#define PHYSICS_H

#include <GLFW/glfw3.h>

#include <my_math/vector.h>

typedef struct State {
    Vector2D position;
    Vector2D velocity;
} State;

Vector2D gravity = { 0.0f, -1.0f };

float t = 0.0;
float dt = 0.01;
    
float currentTime = glfwGetTime();
float accumulator = 0.0;

State state;
State previousState;
State currentState = state;
double prevTime = glfwGetTime();

void IntegrateState(State* state, float time, float dt)
{
    Vector2D velocity = state->velocity;
    Vector2D position = state->position;

    Vector2D acceleration = gravity;

    velocity.x = velocity.x + acceleration.x * dt;
    velocity.y = velocity.y + acceleration.y * dt;

    /*Collision(velocity, state.position, dt); */

    position.x = position.x + velocity.x * dt;
    position.y = position.y + velocity.y * dt;

    state->position = position;
    state->velocity = velocity;
}

void timeStep() {
    // per-frame time logic
        // --------------------
        float newTime = glfwGetTime();

        // using in other stuff
        float deltaTime = newTime - currentTime;

        float frameTime = newTime - currentTime;
        if (frameTime > 0.25)
            frameTime = 0.25;
        float currentTime = newTime;
}

void advanceSimulation()
{
    accumulator += frameTime;

    while (accumulator >= dt) {
        previousState = currentState;
        IntegrateState(currentState, t, dt * devTimeMultiplier);
        t += dt;
        accumulator -= dt;
    }

    const float alpha = accumulator / dt;

    // interpolating between pevious and current state
    state.velocity = currentState.velocity * alpha + previousState.velocity * (1.0f - alpha);
    state.position = currentState.position * alpha + previousState.position * (1.0f - alpha);
}

#endif /* PHYSICS_H */