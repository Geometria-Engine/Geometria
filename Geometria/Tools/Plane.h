#ifndef PLANE_H
#define PLANE_H

#include "Vector.h"
#include "Tools.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Face.h"

struct Plane
{
	Vector3 normal;
	float distanceFromOrigin;
	float a, b, c, d;
	Vector3 pointOnPlane;

	Plane(Vector3 p1, Vector3 p2, Vector3 p3)
    {
        Vector3 ab = Vector3(
        	p2.x - p1.x, 
        	p2.y - p1.y, 
        	p2.z - p1.z);

         Vector3 ac = Vector3(
        	p3.x - p1.x, 
        	p3.y - p1.y, 
        	p3.z - p1.z);

        Vector3 cross = Vector3::Cross(ac, ab);

        normal = Vector3::Normalize(cross);
        distanceFromOrigin = Vector3::Dot(normal, p1);
        pointOnPlane = p1;

        a = normal.x;
        b = normal.y;
        c = normal.z;
        d = -distanceFromOrigin;
    }

    float EvalAtPoint(Vector3 co)
    {
        return a * co.x + b * co.y + c * co.z + d;
    }

    glm::vec3 GetNormal()
    {
        return glm::vec3(normal.x, normal.y, normal.z);
    }

    float GetDistance()
    {
        return distanceFromOrigin;
    }

    bool GetIntersection(Plane& a, Plane& b, glm::vec3& out_Vector)
    {
        float d = glm::dot(GetNormal(), glm::cross(a.GetNormal(), b.GetNormal()));
    
        if (std::fabs(d) < Math::EpsilonDouble()) return false;
    
        out_Vector =
            (
                (glm::cross(a.GetNormal(), b.GetNormal()) * -GetDistance()) -
                (glm::cross(b.GetNormal(), GetNormal()) * a.GetDistance()) -
                (glm::cross(GetNormal(), a.GetNormal()) * b.GetDistance())
            ) / d;
        return true;
    }

    static bool IsVectorInsidePlanes(std::vector<Face>& fa, glm::vec3& v, float margin = 0.1f);
};

#endif