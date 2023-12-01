#ifndef CAMERA_H
#define CAMERA_H

#include <my_math/vector.h>
#include <my_math/matrix.h>

/* Default camera values */
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 10.0f;
const float FOV = 90.0f;

typedef struct Camera {
    Vector2D Position;
    Vector2D Front;
    Vector2D Up;
    Vector2D Right;
    Vector2D WorldUp;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float FOV;
} Camera;

Camera CreateCamera()
{
    Camera camera;

    camera.Front.x = 0.0f;
    camera.Front.y = 0.0f;

    camera.MovementSpeed = SPEED;
    camera.MouseSensitivity = SENSITIVITY;
    camera.Zoom = ZOOM;
    camera.FOV = FOV;

    camera.Position.x = -2.0f;
    camera.Position.y = 0.0f;
    /* y is up */
    camera.WorldUp.x = 0.0f; 
    camera.WorldUp.y = 1.0f; 

    return camera;
}

Mat4* GetViewMatrix(const Camera camera)
{
    Vector3D eye = { camera.Position.x, camera.Position.y, 0.0f };

    Vector2D vec = add_vectors(camera.Position, camera.Front);
    Vector3D center = { vec.x, vec.y, 0.0f };

    Vector3D up = { camera.Up.x, camera.Up.y, 0.0f };

    return lookAt(&eye, &center, &up);
}

#endif /* CAMERA_H */