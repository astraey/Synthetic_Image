#include "sphere.h"

Sphere::Sphere(const double radius_, const Matrix4x4 &t_)
    : Shape(t_), radius(radius_)
{ }

bool Sphere::rayIntersectP(const Ray &ray) const
{
    // Pass the ray to local coordinates
    Ray r = worldToObject.transformRay(ray);

    // The ray-sphere intersection equation can be expressed in the
    // form A*t^2 + B*t + C = 0, where:
	
    double A = VECTOR3D_H::dot(r.d, r.d);
	double B = 2 * r.o.x*r.d.x + 2 * r.o.y*r.d.y + 2 * r.o.z*r.d.z;
    double C = VECTOR3D_H::dot(r.o, r.o) - radius*radius;

    // Now we need to solve this quadratic equation for t
    EqSolver solver;
    rootValues roots;
    bool hasRoots = solver.rootQuadEq(A, B, C, roots);

    if(!hasRoots)
    {
        return false;
    }

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
