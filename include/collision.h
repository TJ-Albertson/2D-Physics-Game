#ifndef COLLISION_H
#define COLLISION_H_H

#include <stdint.h>
#include <stdio.h>

#include <my_math/vector.h>
#include <physics.h>

typedef struct Plane {
    float normal[3];
    float dot;
} Plane;

typedef struct Triangle {
    float points[3];
} Triangle;

typedef struct Object {
    unsigned int index;
} Object;

typedef struct CollisionData{
    float normal[3];
    Triangle triangle;
} CollisionData;

typedef Vector3D Point;

typedef struct AABB {
    Point center; 
    float extents[2];
};  

uint32_t collisionFlags = 0;
State objectState[32];
CollisionData collisionData[32]; 


void CollisionResponse(int objectIndex)
{
    State object_state = objectState[objectIndex];
    CollisionData collision_dat = collisionData[objectIndex];

}

void CheckCollisions(uint32_t collisionFlags, int numDynamicObjects)
{
    for (int i = numDynamicObjects; i >= 0; i--) {

        // Check if the current bit is set (1) or not (0)
        uint32_t mask = 1 << i;
        int colliding = (collisionFlags & mask) ? 1 : 0;

        printf("Bit at position %d: %d\n", i, colliding);

        if (colliding) {
            CollisionResponse(i);
        }
    }
}

void SetCollisionBit(uint32_t *num, int index, int value) 
{
    if (index < 0 || index >= 32) {
        // Handle invalid index
        return;
    }

    // Clear the bit at the specified index
    *num &= ~(1u << index);

    // Set the bit to the desired value
    *num |= (value & 1u) << index;
}

int MovingObject_StaticTriangle_Collision(Object object, Triangle triangle)
{
    return 0;
}

void CollisionDetection(Triangle* triangles, int numTriangles, Object* objects, int numObjects)
{
    for (int i = 0; i < numObjects; ++i) 
    {
        for (int j = 0; j < numTriangles; ++j)
        {
            int collision = MovingObject_StaticTriangle_Collision(objects[i], triangles[j]);

            if (collision)
            {
                SetCollisionBit(&collisionFlags, i, 1);

                collisionData[i].triangle = triangles[j];
            }
        }
    }
}

#endif /* COLLISION_H_H */