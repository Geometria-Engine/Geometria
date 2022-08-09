#include "Plane.h"

bool Plane::IsVectorInsidePlanes(std::vector<Face>& fa, glm::vec3& v, float margin)
{
    for(auto f : fa)
    {
        //std::cout << f.a1.ToString() << f.b2.ToString() << f.c3.ToString() << std::endl;
        Plane p = Plane(f.a1, f.b2, f.c3);
        float distance = (glm::dot(p.GetNormal(), v) - p.GetDistance()) - margin;
        if(distance > 0)
        {
            //std::cout << "Distance is " << distance << "! Return False!" << std::endl;
            return false;
        }
    }
    return true;
}