#include "sphere.h"

Sphere::Sphere(const double radius_, const Matrix4x4 &t_)
    : Shape(t_), radius(radius_)
{ }

bool Sphere::rayIntersectP(const Ray &ray) const
{
    // Pass the ray to local coordinates
    Ray r = worldToObject.transformRay(ray);

    std::cout << "PLEASE COMPLETE THE Sphere::rayIntersectP() FUNCTION!" << std::endl;

    // The ray-sphere intersection equation can be expressed in the
    // form A*t^2 + B*t + C = 0, where:
    /*double A = ;
    double B = ;
    double C = ;

    // Now we need to solve this quadratic equation for t
    EqSolver solver;
    rootValues roots;
    bool hasRoots = solver.rootQuadEq(A, B, C, roots);

    if(!hasRoots)
    {
        return false;
    }*/

    return true;
}

std::string Sphere::toString() const
{
    std::stringstream s;
    s << "[ " << std::endl
      << " Center (World) = " << objectToWorld.transformPoint(Vector3D(0,0,0)) << ", Radius = " << radius << std::endl
      << "]" << std::endl;

    return s.str();
}

std::ostream& operator<<(std::ostream &out, const Sphere &s)
{
    out << s.toString();
    return out;
}
