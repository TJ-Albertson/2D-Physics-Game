#ifndef COLLISION_H
#define COLLISION_H_H

#include <stdint.h>
#include <stdio.h>

#include <my_math/vector.h>
#include <physics.h>

const float EPSILON = 1e-6f;
const float FLT_MAX = 3.40282347e+38F;

typedef Vector2D Point2D;

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


typedef struct Segment {
    Point2D p1;
    Point2D p2;
} Segment;


/* Shapes */
typedef struct AABB {
    Point2D min;
    Point2D max;
} AABB;  

typedef struct Sphere {
    Point2D center;
    float radius;
} Sphere;  

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
    int i;

    for (i = numDynamicObjects; i >= 0; i--) {

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
        return;
    }

    *num &= ~(1u << index);
    *num |= (value & 1u) << index;
}

int MovingObject_StaticTriangle_Collision(Object object, Triangle triangle)
{
    return 0;
}

void CollisionDetection(Triangle* triangles, int numTriangles, Object* objects, int numObjects)
{
    int i, j;

    for (i = 0; i < numObjects; ++i) 
    {
        for (j = 0; j < numTriangles; ++j)
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



/*
   Intersect ray R(t) = p + t*d against AABB a. When intersecting,
   return intersection distance tmin and point q of intersection
*/
int IntersectRayAABB(Point2D point, Vector2D d, AABB aabb, float* tmin, Point2D* q)
{
    (*tmin) = 0.0f;          /* set to -FLT_MAX to get first hit on line */
    float tmax = FLT_MAX;    /* set to max distance ray can travel (for segment) */

    int i;
    /* For all three slabs */
    for (i = 0; i < 2; i++)
    {
        if (my_abs(d.data[i]) < EPSILON)
        {
            /* Ray is parallel to slab. No hit if origin not within slab */
            if (point.data[i] < aabb.min.data[i] || point.data[i] > aabb.max.data[i])
                return 0;
        }
        else
        {
            /* Compute intersection t value of ray with near and far plane of slab */
            float ood = 1.0f / d.data[i];
            float t1 = (aabb.min.data[i] - point.data[i]) * ood;
            float t2 = (aabb.max.data[i] - point.data[i]) * ood;

            /* Make t1 be intersection with near plane, t2 with far plane */
            if (t1 > t2) swap_float(&t1, &t2);

            /* Compute the intersection of slab intersection intervals */
            if (t1 > (*tmin)) (*tmin) = t1;
            if (t2 > tmax) (*tmin) = t2;

            /* Exit with no collision as soon as slab intersection becomes empty */
            if ((*tmin) > tmax)
                return 0;
        }
    }

    /* Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin) */
    (*q).x = point.x + d.x * (*tmin);
    (*q).y = point.y + d.y * (*tmin);
    return 1;
}



/* Intersection test between a 2D segment and a 2D capsule with semi-circle endcaps */
int IntersectSegmentCapsule(Segment segment, Point2D p, Point2D q, float r, float *t)
{
    Vector2D d = {q.x - p.x, q.y - p.y};
    Vector2D m = {segment.p1.x - p.x, segment.p1.y - p.y};
    Vector2D n = {segment.p2.x - segment.p1.x, segment.p2.y - segment.p1.y};

    float md = m.x * d.x + m.y * d.y;
    float nd = n.x * d.x + n.y * d.y;
    float dd = d.x * d.x + d.y * d.y;

    /* Test if segment is fully outside either endcap of the capsule */
    if (md < 0.0f && md + nd < 0.0f)
        return 0; /* Segment outside 'p' side of the capsule */
    if (md > dd && md + nd > dd)
        return 0; /* Segment outside 'q' side of the capsule */

    float nn = n.x * n.x + n.y * n.y;
    float mn = m.x * n.x + m.y * n.y;
    float a = dd * nn - nd * nd;
    float k = m.x * m.x + m.y * m.y - r * r;
    float c = dd * k - md * md;

    const float EPSILON = 1e-5f;

    if (my_fabs(a) < EPSILON)
    {
        /* Segment runs parallel to capsule axis */
        if (c > 0.0f)
            return 0; /* 'a' and thus the segment lie outside capsule */

        /* Now known that segment intersects capsule; figure out how it intersects */
        if (md < 0.0f)
            (*t) = -mn / nn; /* Intersect segment against 'p' endcap */
        else if (md > dd)
            (*t) = (nd - mn) / nn; /* Intersect segment against 'q' endcap */
        else
            (*t) = 0.0f; /* 'a' lies inside capsule */

        return 1;
    }

    float b = dd * mn - nd * md;
    float discr = b * b - a * c;

    if (discr < 0.0f)
        return 0; /* No real roots; no intersection */

    (*t) = (-b - my_sqrt(discr)) / a;

    if ((*t) < 0.0f || (*t) > 1.0f)
        return 0; /* Intersection lies outside segment */

    if (md + (*t) * nd < 0.0f)
    {
        /* Intersection outside capsule on 'p' side */
        if (nd <= 0.0f)
            return 0; /* Segment pointing away from endcap */

        (*t) = -md / nd;

        /* Keep intersection if Dot(S(t) - p, S(t) - p) <= r^2 */
        return k + 2 * (*t) * (mn + (*t) * nn) <= 0.0f;
    }
    else if (md + (*t) * nd > dd)
    {
        /* Intersection outside capsule on 'q' side */
        if (nd >= 0.0f)
            return 0; /* Segment pointing away from endcap */

        (*t) = (dd - md) / nd;

        /* Keep intersection if Dot(S(t) - q, S(t) - q) <= r^2 */
        return k + dd - 2 * md + (*t) * (2 * (mn - nd) + (*t) * nn) <= 0.0f;
    }

    /* Segment intersects capsule between the endcaps; t is correct */
    return 1;
}



/* Support function that returns the AABB vertex with index n */
Point2D Corner(AABB box, int index)
{
    Point2D cornerPoint;
    cornerPoint.x = ((index & 1) ? box.max.x : box.min.x);
    cornerPoint.y = ((index & 1) ? box.max.y : box.min.y);
    return cornerPoint;
}

int IntersectMovingSphereAABB(Sphere sphere, Vector2D direction, AABB box, float* intersection_time)
{
    /* Compute the AABB resulting from expanding b by sphere radius r */
    AABB expanded_box = box;
    expanded_box.min.x -= sphere.radius; 
    expanded_box.min.y -= sphere.radius;
    expanded_box.max.x += sphere.radius; 
    expanded_box.max.y += sphere.radius;

    /*
    Intersect ray against expanded AABB e. Exit with no intersection if ray
    misses e, else get intersection point p and time t as result
    */
    Point2D intersection_point;
    if ( !IntersectRayAABB(sphere.center, direction, expanded_box, intersection_time, &intersection_point) || (*intersection_time) > 1.0f )
        return 0;

    /*
    Compute which min and max faces of b the intersection point p lies
    outside of. Note, u and v cannot have the same bits set and
    they must have at least one bit set among them
    */
    /*  outside_min = 00, outside_max = 00*/
    int outside_min = 0, outside_max = 0;
    if (intersection_point.x < box.min.x) outside_min |= 1;
    if (intersection_point.y < box.min.y) outside_min |= 2;

    if (intersection_point.x > box.max.x) outside_max |= 1;
    if (intersection_point.y > box.max.y) outside_max |= 2;

    /* ‘Or’ all set bits together into a bit mask (note: here u + v == u | v) */
    int bitmask = outside_min + outside_max;

    /* Define line segment [c, c+d] specified by the sphere movement */
    Segment segment;
    segment.p1 = sphere.center;
    segment.p2 = add_2d_vectors(sphere.center, direction);
    
    /* If all 3 bits set (m == 3) then p is in a vertex region */
    if (bitmask == 3) {
        /* Must now intersect segment [c, c+d] against the capsules of the two */
        /* edges meeting at the vertex and return the best time, if one or more hit */
        float minimum_time = FLT_MAX;
        if ( IntersectSegmentCapsule(segment, Corner(box, outside_max), Corner(box, outside_max ^ 1), sphere.radius, intersection_time))
            minimum_time = my_min((*intersection_time), minimum_time);
        if ( IntersectSegmentCapsule(segment, Corner(box, outside_max), Corner(box, outside_max ^ 2), sphere.radius, intersection_time))
            minimum_time = my_min((*intersection_time), minimum_time);
            

        if (minimum_time == FLT_MAX) return 0; /* No intersection */
        (*intersection_time) = minimum_time;
        return 1; /* Intersection at time t == tmin */
    }

    /* If only one bit set in m, then p is in a face region */
    if ((bitmask & (bitmask - 1)) == 0)
    {
        /*
            Do nothing. Time t from intersection with
            expanded box is correct intersection time
        */
        return 1;
    }

    /* p is in an edge region. Intersect against the capsule at the edge */
    return IntersectSegmentCapsule(segment, Corner(box, outside_min ^ 3), Corner(box, outside_max), sphere.radius, intersection_time);
}


int playerColliding = 0;

void PlayerCollision(State* state, AABB aabb) 
{
    Sphere collisionSphere;
    collisionSphere.center = state->position;
    collisionSphere.radius = 0.5f;

    float time_of_collision;

    Vector2D velocity;
    velocity.x = state->velocity.x * 0.01f;
    velocity.y = state->velocity.y * 0.01f;

    int collision = IntersectMovingSphereAABB(collisionSphere, velocity, aabb, &time_of_collision);
    playerColliding = collision;

    if (collision)
    {
        printf("!!!collision!!!\n");
    }
}



#endif /* COLLISION_H_H */