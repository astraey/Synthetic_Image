#include "moth.h"

#include "../core/utils.h"

Moth::Moth(const double radius_, const Matrix4x4 &t_, Material *material_)
	: Shape(t_, material_), radius(radius_), position(t_)
{ }

double Moth::getRadius() const
{
	return radius;
}

Matrix4x4 Moth::getPosition() const
{
	return this->position;
}

void Moth::setPosition(Matrix4x4 position)
{

	this->position = position;

}


// Return the normal in world coordinates
// Pre condition: the point passed as argument to this function is in
// world coordinates and belongs to the sphere
Vector3D Moth::getNormalWorld(const Vector3D &pt_world) const
{
    // Transform the point to local coordinates
    Vector3D pt_local = worldToObject.transformPoint(pt_world);

    // CHECK IF THE POINT EFFECTIVELLY BELONGS TO THE SPHERE?
    // TODO?

    // Normal in local coordinates
    Vector3D n(pt_local.x, pt_local.y, pt_local.z);

    // Transform the normal to world coordinates
    //Normal nWorld = objectToWorld.applyTransform(n);
    // Multiply the normal by the transpose of the inverse
    Matrix4x4 inverseTransposed;
    worldToObject.transpose(inverseTransposed);
    Vector3D nWorld = inverseTransposed.transformVector(n);

    // Check whether applying the transform to a normalized
    // normal allways yields a normalized normal
    return(nWorld.normalized());
}


// Chapter 3 PBRT, page 117
bool Moth::rayIntersect(const Ray &ray, Intersection &its) const
{
    // Pass the ray to local coordinates
    //Ray r = worldToObject.applyTransform(ray);
    Ray r = worldToObject.transformRay(ray);

    // The ray-sphere intersection equation can be expressed in the
    // form A*t^2 + B*t + C = 0, where:
    double A = r.d.x*r.d.x + r.d.y*r.d.y + r.d.z*r.d.z;
    double B = 2*(r.o.x*r.d.x + r.o.y*r.d.y + r.o.z*r.d.z);
    double C = r.o.x*r.o.x + r.o.y*r.o.y +
               r.o.z*r.o.z - radius*radius;

    // Now we need to solve this quadratic equation for t
    EqSolver solver;
    rootValues roots;
    bool hasRoots = solver.rootQuadEq(A, B, C, roots);

    if(!hasRoots)
    {
        return false;
    }

    // Test whether both root values (i.e., the intersection points)
    // are greater or smaller than r.maxT and r.minT, respectivelly
    if (roots.values[0] > r.maxT || roots.values[1] < r.minT)
        return false;

    // If they are not, then there are three possibilities:
    // 1 - t0 > minT and t1 > maxT, in which case t0 is the hit point we want to retain
    // 2 - t0 < minT and t1 < maxT, in which case t1 is the hit point we want to retain
    // 3 - Both roots are out of the ray segment (t0 < minT and t1 > maxT)

    // We initialize the tHit for case 1
    double tHit = roots.values[0];
    // We check where we could possibly be in case 2 or 3
    if (roots.values[0] < ray.minT) {
        // If so, then we set tHit to t1 (hoping we are in case 2!)
        tHit = roots.values[1];
        // If we are in case 3, then return false (meaning there is no intersection detected
        if (tHit > ray.maxT)
            return false;
    }

    // If we arrive here it is because there is an intersection
    // with the tested ray segment!

    // Update the maxT in the ray so as to terminate earlier subsequent
    // intersection tests with other shapes
    ray.maxT = tHit;

    // Compute Intersection Point (in local coordinates)
    Vector3D itsPoint = r.o + r.d*tHit;

    // Transform to world coordinates
    its.itsPoint = objectToWorld.transformPoint(itsPoint);

    // Compute the normal at the intersection point (in world coordinates)
    its.normal   = getNormalWorld(its.itsPoint);

    // Store the shape the intersection point lies in
    its.shape = this;

    return true;
}


// Chapter 3 PBRT, page 117
bool Moth::rayIntersectP(const Ray &ray) const
{
    // Pass the ray to local coordinates
    Ray r = worldToObject.transformRay(ray);

    // The ray-sphere intersection equation can be expressed in the
    // form A*t^2 + B*t + C = 0, where:
    double A = r.d.x*r.d.x + r.d.y*r.d.y + r.d.z*r.d.z;
    double B = 2*(r.o.x*r.d.x + r.o.y*r.d.y + r.o.z*r.d.z);
    double C = r.o.x*r.o.x + r.o.y*r.o.y +
               r.o.z*r.o.z - radius*radius;

    // Now we need to solve this quadratic equation for t
    EqSolver solver;
    rootValues roots;
    bool hasRoots = solver.rootQuadEq(A, B, C, roots);

    if(!hasRoots)
    {
        return false;
    }

    // Test whether both root values (i.e., the intersection points)
    // are greater or smaller than r.maxT and r.minT, respectivelly
    if (roots.values[0] > r.maxT || roots.values[1] < r.minT)
        return false;

    // If they are not, then there are three possibilities:
    // 1 - t0 > minT and t1 > maxT, in which case t0 is the hit point we want to retain
    // 2 - t0 < minT and t1 < maxT, in which case t1 is the hit point we want to retain
    // 3 - Both roots are out of the ray segment (t0 < minT and t1 > maxT)

    // We initialize the tHit for case 1
    double tHit = roots.values[0];
    // We check where we could possibly be in case 2 or 3
    if (roots.values[0] < ray.minT) {
        // If so, then we set tHit to t1 (hoping we are in case 2!)
        tHit = roots.values[1];
        // If we are in case 3, then return false (meaning there is no intersection detected
        if (tHit > ray.maxT)
            return false;
    }

    // If we arrive here it is because there is an intersection
    // with the tested ray segment!

    // Update the maxT in the ray so as to terminate earlier subsequent
    // intersection tests with other shapes
    ray.maxT = tHit;

    return true;
}


int Moth::mothDirection(const std::vector<Vector3D> &pointList, const std::vector<Shape*> &objList,
	const std::vector<PointLightSource> &lsList) const
{
	/*
	std::cout << "Point List SIZE**** :" << pointList.size() << std::endl;
	std::cout << "**" << pointList[0] << std::endl;
	std::cout << "**" << pointList[1] << std::endl;
	std::cout << "**" << pointList[2] << std::endl;
	std::cout << "**" << pointList[3] << std::endl;
	std::cout << "**" << pointList[4] << std::endl;
	std::cout << "**" << pointList[5] << std::endl;
	*/


	int nL = lsList.size();

	//std::cout << "Light List Size :" << lsList.size() << std::endl;
	//std::cout << "PointList :" << pointList.size() << std::endl;

	Ray R;
	Vector3D wi;
	
	std::vector<Vector3D> Illumination;



	for (int np = 0; np < pointList.size(); np++)
	{

		Vector3D P = pointList[np];
		//std::cout << "P :" << pointList[np] << std::endl;


		Vector3D wo = -this->getNormalWorld(P).normalized();

		//std::cout << "P :" << P << std::endl;
		//std::cout << "wo :" << wo << std::endl;


		for (int i = 0; i < nL; i++)
		{

			Vector3D D = lsList[i].getPosition() - P;
			wi = D.normalized();

			R = Ray(P, wi);
			R.maxT = D.length();

			Intersection its2;

			Utils::getClosestIntersection(R, objList, its2);

			if ((Utils::hasIntersection(R, objList)) || its2.shape->getMaterial().hasTransmission())
			{
				//std::cout << "WWEEEEE ENTEREE HEREEE" << std::endl;




					//std::cout << "WWEEEEE ENTEREE HEREEE" << std::endl;

					Vector3D I = lsList[i].getIntensity(P);

					//std::cout <<"Intensity: "<< I << std::endl;

					//Vector3D R = this->getMaterial().getReflectance(this->getNormalWorld(P), wo, wi);


					//Illumination[np] = Utils::multiplyPerCanal(I, R);
					//Illumination.push_back(Utils::multiplyPerCanal(I, R));
					Illumination.push_back(I);


				
			}
		}
	}

	/*
	std::cout << "Ilumination SIZE**** :" << Illumination.size() << std::endl;
	std::cout << "**" << Illumination[0] << std::endl;
	std::cout << "**" << Illumination[1] << std::endl;
	std::cout << "**" << Illumination[2] << std::endl;
	std::cout << "**" << Illumination[3] << std::endl;
	std::cout << "**" << Illumination[4] << std::endl;
	std::cout << "**" << Illumination[5] << std::endl;
	*/


	double max = Illumination[0].x + Illumination[0].y + Illumination[0].z;


	int maxpos = 0;


	for (int j = 0; j < Illumination.size(); j++)
	{

		double compare = Illumination[j].x + Illumination[j].y + Illumination[j].z;


		//std::cout << "ENTERED HERE" << compare << std::endl;


			if (max <= compare)
			{
				//std::cout << "ENTERED HERE in iteration " << j << " MAX: " << max << "Compare: " << compare << std::endl;
				max = compare;
				maxpos = j;

			}
	}

	return maxpos;

}


std::string Moth::toString() const
{
    std::stringstream s;
    s << "[ " << std::endl
      << " Center (World) = " << objectToWorld.transformPoint(Vector3D(0,0,0)) << ", Radius = " << radius << std::endl
      << "]" << std::endl;

    return s.str();
}

std::ostream& operator<<(std::ostream &out, const Moth &s)
{
    out << s.toString();
    return out;
}
