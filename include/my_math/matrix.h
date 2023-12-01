#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

#include <my_math/basic.h>
#include <my_math/vector.h>

typedef float Mat4[4][4];

Mat4* perspective(float fovy, float aspect, float near, float far) 
{
    Mat4* matrix = (Mat4*)malloc(sizeof(Mat4));
    float f = 1.0f / my_tanf(fovy / 2.0f);

    (*matrix)[0][0] = f / aspect;
    (*matrix)[0][1] = 0.0f;
    (*matrix)[0][2] = 0.0f;
    (*matrix)[0][3] = 0.0f;

    (*matrix)[1][0] = 0.0f;
    (*matrix)[1][1] = f;
    (*matrix)[1][2] = 0.0f;
    (*matrix)[1][3] = 0.0f;

    (*matrix)[2][0] = 0.0f;
    (*matrix)[2][1] = 0.0f;
    (*matrix)[2][2] = (far + near) / (near - far);
    (*matrix)[2][3] = -1.0f;

    (*matrix)[3][0] = 0.0f;
    (*matrix)[3][1] = 0.0f;
    (*matrix)[3][2] = (2.0f * far * near) / (near - far);
    (*matrix)[3][3] = 0.0f;

    return matrix;
}

Mat4* lookAt(const Vector3D *eye, const Vector3D *center, const Vector3D *up) 
{
    Mat4* matrix = (Mat4*)malloc(sizeof(Mat4));
    Vector3D f, u, r;

    /* Calculate forward vector */ 
    subtract(&f, center, eye);
    normalize(&f);

    /* Calculate right vector */ 
    cross(&r, up, &f);
    normalize(&r);

    /* Calculate up vector */ 
    cross(&u, &f, &r);
    normalize(&u);

    /* Set the matrix elements */ 
    (*matrix)[0][0] = r.x;
    (*matrix)[0][1] = u.x;
    (*matrix)[0][2] = -f.x;
    (*matrix)[0][3] = 0.0f;

    (*matrix)[1][0] = r.y;
    (*matrix)[1][1] = u.y;
    (*matrix)[1][2] = -f.y;
    (*matrix)[1][3] = 0.0f;

    (*matrix)[2][0] = r.z;
    (*matrix)[2][1] = u.z;
    (*matrix)[2][2] = -f.z;
    (*matrix)[2][3] = 0.0f;

    (*matrix)[3][0] = -r.x * eye->x - r.y * eye->y - r.z * eye->z;
    (*matrix)[3][1] = -u.x * eye->x - u.y * eye->y - u.z * eye->z;
    (*matrix)[3][2] = f.x * eye->x + f.y * eye->y + f.z * eye->z;
    (*matrix)[3][3] = 1.0f;

    return matrix;
}

#endif /* MATRIX_H */