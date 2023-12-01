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

typedef enum  {
    ORTHOGRAPHIC,
    PERSPECTIVE
} Camera_Projection;

typedef struct {
    Vector3D Position;
    Vector2D Front;
    Vector2D Up;
    Vector2D Right;
    Vector2D WorldUp;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float FOV;

    Camera_Projection Projection;   
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

    camera.Position.x = 0.0f;
    camera.Position.y = 0.0f;
    camera.Position.z = 1.0f;
    /* y is up */
    camera.WorldUp.x = 0.0f; 
    camera.WorldUp.y = 1.0f; 

    camera.Projection = ORTHOGRAPHIC;

    return camera;
}

Mat4* GetViewMatrix(const Camera camera)
{
    Vector3D position = { camera.Position.x, camera.Position.y, camera.Position.z };

    Vector3D temp = { camera.Front.x, camera.Front.y, 0.0f };

    Vector3D target = add_3d_vectors(camera.Position, temp);
    target.z = 0.0f;

    Vector3D up = { camera.WorldUp.x, camera.WorldUp.y, 0.0f };

    return lookAt(&position, &target, &up);
}

#endif /* CAMERA_H */