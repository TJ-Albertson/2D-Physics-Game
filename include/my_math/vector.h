#ifndef VECTOR_H
#define VECTOR_H

#include "my_math/basic.h"

typedef struct Vector2D {
    union {
        struct {
            float x, y;
        };
        float data[2];
    };
} Vector2D;

typedef struct Vector3D
{
    float x;
    float y;
    float z;
}
Vector3D;

void normalize(Vector3D *v) {
    float length = my_sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

void cross(Vector3D *result, const Vector3D *v1, const Vector3D *v2) {
    result->x = v1->y * v2->z - v1->z * v2->y;
    result->y = v1->z * v2->x - v1->x * v2->z;
    result->z = v1->x * v2->y - v1->y * v2->x;
}

void subtract(Vector3D *result, const Vector3D *v1, const Vector3D *v2) {
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
}

Vector2D add_2d_vectors(Vector2D v1, Vector2D v2) {
    Vector2D result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

Vector3D add_3d_vectors(Vector3D v1, Vector3D v2) {
    Vector3D result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

#endif /* VECTOR_H */