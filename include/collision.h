#ifndef COLLISION_H
#define COLLISION_H_H

#include <stdint.h>
#include <stdio.h>

#include <my_math/vector.h>
#include <physics.h>

const float EPSILON = 1e-6f;
const float FLT_MAX = 3.40282347e+38F;

typedef Vector2D Point;

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
    Point p1;
    Point p2;
} Segment;


/* Shapes */
typedef struct AABB {
    Point min;
    Point max;
} AABB;  

typedef struct Sphere {
    Point center;
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
    for (int i = numDynamicObjects; i >= 0; i--) {

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


void PlayerCollisionDetection(State state)
{

}

/*
   Intersect ray R(t) = p + t*d against AABB a. When intersecting,
   return intersection distance tmin and point q of intersection
*/
int IntersectRayAABB(Point point, Vector2D d, AABB aabb, float* tmin, Point &q)
{
    (*tmin) = 0.0f;          /* set to -FLT_MAX to get first hit on line */
    float tmax = FLT_MAX;    /* set to max distance ray can travel (for segment) */

    /* For all three slabs */
    for (int i = 0; i < 2; i++)
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
    q.x = point.x + d.x * (*tmin);
    q.y = point.y + d.y * (*tmin);
    return 1;
}



/* Intersection test between a 2D segment and a 2D capsule with semi-circle endcaps */
int IntersectSegmentCapsule(Segment segment, Point p, Point q, float r, float *t)
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
Point Corner(AABB b, int n)
{
    Point p;
    p.x = ((n & 1) ? b.max.x : b.min.x);
    p.y = ((n & 1) ? b.max.y : b.min.y);
    return p;
}

int IntersectMovingSphereAABB(Sphere sphere, Vector2D d, AABB b, float* t)
{
    /* Compute the AABB resulting from expanding b by sphere radius r */
    AABB aabb = b;
    aabb.min.x -= sphere.radius;
    aabb.min.y -= sphere.radius;
    aabb.max.x += sphere.radius;
    aabb.max.y += sphere.radius;

    /*
    Intersect ray against expanded AABB e. Exit with no intersection if ray
    misses e, else get intersection point p and time t as result
    */
    Point p;
    if ( !IntersectRayAABB(sphere.center, d, aabb, t, p) || (*t) > 1.0f )
        return 0;

    /*
    Compute which min and max faces of b the intersection point p lies
    outside of. Note, u and v cannot have the same bits set and
    they must have at least one bit set among them
    */
    int u = 0, v = 0;
    if (p.x < b.min.x) u |= 1;
    if (p.x > b.max.x) v |= 1;
    if (p.y < b.min.y) u |= 2;
    if (p.y > b.max.y) v |= 2;

    /* ‘Or’ all set bits together into a bit mask (note: here u + v == u | v) */
    int m = u + v;

    /* Define line segment [c, c+d] specified by the sphere movement */
    Segment segment;
    segment.p1 = sphere.center;
    segment.p2 = add_2d_vectors(sphere.center, d);
    

    /* If all 3 bits set (m == 3) then p is in a vertex region */
    if (m == 3) {

        /* Must now intersect segment [c, c+d] against the capsules of the two */
        /* edges meeting at the vertex and return the best time, if one or more hit */
        float tmin = FLT_MAX;
        if ( IntersectSegmentCapsule(segment, Corner(b, v), Corner(b, v ^ 1), sphere.radius, t))
            tmin = my_min((*t), tmin);

        if (tmin == FLT_MAX) return 0; /* No intersection */
        (*t) = tmin;
        return 1; /* Intersection at time t == tmin */
    }

    /* If only one bit set in m, then p is in a face region */
    if ((m & (m - 1)) == 0)
    {
        /*
            Do nothing. Time t from intersection with
            expanded box is correct intersection time
        */
        return 1;
    }

    /* p is in an edge region. Intersect against the capsule at the edge */
    return IntersectSegmentCapsule(segment, Corner(b, u & 3), Corner(b, v), sphere.radius, t);
}



#endif /* COLLISION_H_H */