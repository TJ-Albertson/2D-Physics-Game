#ifndef PHYSICS_H
#define PHYSICS_H

#include <GLFW/glfw3.h>

#include <my_math/vector.h>
#include <input.h>

typedef struct State {
    Vector2D position;
    Vector2D velocity;
} State;

Vector2D gravity = { 0.0f, -0.008f };

float t = 0.0;
float dt = 0.01;

State state;

/*

void IntegrateState(State* state, float time, float dt)
{
    Vector2D velocity = state->velocity;
    Vector2D position = state->position;

    Vector2D acceleration = gravity;

    velocity.x = velocity.x + acceleration.x * dt;
    velocity.y = velocity.y + acceleration.y * dt;

    /*Collision(velocity, state.position, dt); */ /*

    position.x = position.x + velocity.x * dt;
    position.y = position.y + velocity.y * dt;

    state->position = position;
    state->velocity = velocity;
}

   
void timeStep() 
{
    float newTime = glfwGetTime();

    float deltaTime = newTime - currentTime;

    float frameTime = newTime - currentTime;
    if (frameTime > 0.25)
        frameTime = 0.25;
    float currentTime = newTime;
}

void advanceSimulation()
{
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
    /*
    state.velocity.x = currentState.velocity.x * alpha + previousState.velocity.x * (1.0f - alpha);
    state.velocity.y = currentState.velocity.y * alpha + previousState.velocity.y * (1.0f - alpha);

    state.position.x = currentState.position.x * alpha + previousState.position.x * (1.0f - alpha);
    state.position.y = currentState.position.y * alpha + previousState.position.y * (1.0f - alpha);
}*/

#endif /* PHYSICS_H */