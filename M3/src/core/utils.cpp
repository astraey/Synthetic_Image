#include "utils.h"

double step = 0.03;
double stepLight = 0.05;

Utils::Utils()
{ }

double Utils::degreesToRadians(double degrees)
{
	return degrees * M_PI / 180.0;
}

Vector3D Utils::multiplyPerCanal(const Vector3D &v1, const Vector3D &v2)
{
	return Vector3D(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

bool Utils::hasIntersection(const Ray &ray, const std::vector<Shape*> &objectsList)
{

	// For each object on the scene...
	for (size_t objIndex = 0; objIndex < objectsList.size(); objIndex++)
	{
		// Get the current object
		const Shape *obj = objectsList.at(objIndex);

		if ((*obj).rayIntersectP(ray))
			return true;

	}

	return false;
}

bool Utils::getClosestIntersection(const Ray &cameraRay, const std::vector<Shape*> &objectsList, Intersection &its)
{

	// For each object on the scene...
	bool hasIntersection = false;

	for (size_t objIndex = 0; objIndex < objectsList.size(); objIndex++)
	{
		// Get the current object
		const Shape *obj = objectsList.at(objIndex);

		if ((*obj).rayIntersect(cameraRay, its))
		{
			hasIntersection = true;
		}

	}

	return hasIntersection;
}

double interpolate(double val, double y0, double x0, double y1, double x1)
{
	return (val - x0)*(y1 - y0) / (x1 - x0) + y0;
}

double getRed(double value)
{
	if (value > 0.5)
		return interpolate(value, 0.0, 0.5, 1.0, 1.0);
	else
		return 0.0;
}

double getGreen(double value)
{
	if (value < 0.25)
		return 0.0;
	else if (value < 0.5)
		return interpolate(value, 0.0, 0.25, 1, 0.5);
	else if (value < 0.75)
		return interpolate(value, 1, 0.5, 0, 0.75);
	else
		return 0;
}

double getBlue(double value)
{
	if (value < 0.5)
		return interpolate(value, 1.0, 0.0, 0.0, 0.5);
	else
		return 0;
}


Vector3D Utils::scalarToRGB(double scalar)
{
	return Vector3D(getRed(scalar),
		getGreen(scalar),
		getBlue(scalar));
}

Vector3D Utils::computeReflectionDirection(const Vector3D &rayDirection, const Vector3D &normal)
{
	// Compute the perfect reflection direction
	return normal * 2 * dot(normal, -rayDirection) + rayDirection;
}

bool Utils::isTotalInternalReflection(const double &eta, const double &cosThetaI,
	double &cosThetaT_out)
{
	double r = 1 + (pow(eta, 2) * (pow(cosThetaI, 2) - 1));

	if (r >= 0)
		cosThetaT_out = sqrt(r);
	else
		cosThetaT_out = cosThetaI;

	return (r < 0);
}

Vector3D Utils::computeTransmissionDirection(const Ray &r, const Vector3D &normal,
	const double &eta, const double &cosThetaI,
	const double &cosThetaT)
{
	return r.d * eta + normal * (eta * cosThetaI - cosThetaT);
}

double Utils::computeReflectanceCoefficient(const double &eta, const double &cosThetaI,
	const double &cosThetaT)
{
	return 0.0;
}

Vector3D Utils::moveUp(Vector3D pos)
{
	return Vector3D(pos.x, pos.y + step, pos.z);
}

Vector3D Utils::moveDown(Vector3D pos)
{
	return Vector3D(pos.x, pos.y - step, pos.z);
}

Vector3D Utils::moveLeft(Vector3D pos)
{
	return Vector3D(pos.x - step, pos.y, pos.z);
}

Vector3D Utils::moveRight(Vector3D pos)
{
	return Vector3D(pos.x + step, pos.y, pos.z);
}

Vector3D Utils::moveForward(Vector3D pos)
{
	return Vector3D(pos.x, pos.y, pos.z + step);
}

Vector3D Utils::moveBackwards(Vector3D pos)
{
	return Vector3D(pos.x, pos.y, pos.z - step);
}

Vector3D Utils::moveUpLight(Vector3D pos)
{
	return Vector3D(pos.x, pos.y + stepLight, pos.z);
}

Vector3D Utils::moveDownLight(Vector3D pos)
{
	return Vector3D(pos.x, pos.y - stepLight, pos.z);
}

Vector3D Utils::moveLeftLight(Vector3D pos)
{
	return Vector3D(pos.x - stepLight, pos.y, pos.z);
}

Vector3D Utils::moveRightLight(Vector3D pos)
{
	return Vector3D(pos.x + stepLight, pos.y, pos.z);
}

Vector3D Utils::moveForwardLight(Vector3D pos)
{
	return Vector3D(pos.x, pos.y, pos.z + stepLight);
}

Vector3D Utils::moveBackwardsLight(Vector3D pos)
{
	return Vector3D(pos.x, pos.y, pos.z - stepLight);
}