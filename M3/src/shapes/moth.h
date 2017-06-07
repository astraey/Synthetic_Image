#ifndef MOTH_H
#define MOTH_H

#include <iostream>
#include <string>
#include <vector>

#include "shape.h"
#include "../core/eqsolver.h"
#include "../core/ray.h"
#include "../lightsources/pointlightsource.h"
#include "../shapes/shape.h"

class Moth : public Shape
{
public:
    Moth() = delete;
    Moth(const double radius_, const Matrix4x4 &t, Material *material_);

	double getRadius() const;
	Matrix4x4 getPosition() const;

	void setPosition(Matrix4x4 position);

    Vector3D getNormalWorld(const Vector3D &pt_world) const;

    virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
    virtual bool rayIntersectP(const Ray &ray) const;

	virtual int mothDirection(const std::vector<Vector3D> &pointList, const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;

    std::string toString() const;

private:
    // The center of the sphere in local coordinates is assumed
    // to be (0, 0, 0). To pass to world coordinates just apply the
    // objectToWorld transformation contained in the mother class
    double radius;
	Matrix4x4 position;
};

std::ostream& operator<<(std::ostream &out, const Moth &s);

#endif // SPHERE_H
