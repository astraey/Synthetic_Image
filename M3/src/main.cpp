
#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/moth.h"
#include "shapes/infiniteplane.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/directshader.h"
#include "shaders/normalshader.h"
#include "shaders/globalshader.h"
#include "materials/phong.h"
#include "materials/mirror.h"
#include "materials/lamp.h"
#include "materials/transmissive.h"

int frameCounter = 0;

Vector3D mothPosition = Vector3D(-0.75, -0.75, 4.5);

Vector3D lampPosition1 = Vector3D(0.3, 1.5, 3.5);
bool lampDirection1 = true;

Vector3D lampPosition2 = Vector3D(0.3, -0.7, 3.5);
bool lampDirection2 = true;

//std::vector<Moth*>* mothsList = new std::vector<Moth*>;

void buildSceneSphere(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList,
	std::vector<PointLightSource>* &lightSourceList,
	std::vector<Moth*> &mothList)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	// By default, this gives an ID transform
	//  which means that the camera is located at (0, 0, 0)
	//  and looking at the "+z" direction
	Matrix4x4 cameraToWorld;
	cameraToWorld.translate(Vector3D(0, 0, -30));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ************************** */
	/* DEFINE YOUR MATERIALS HERE */
	/* ************************** */
	Material *green_50 = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 80);
	Material *red_50 = new Phong(Vector3D(0.7, 0.2, 0.2), Vector3D(0.6, 0.2, 0.2), 80);
	Material *blue_50 = new Phong(Vector3D(0.2, 0.2, 0.5), Vector3D(0.2, 0.2, 0.4), 80);
	Material *white_50 = new Phong(Vector3D(0.9, 0.9, 0.9), Vector3D(0.8, 0.8, 0.8), 80);

	Material *moth_50 = new Phong(Vector3D(0.1, 0.1, 0.1), Vector3D(0.1, 0.1, 0.1), 10);

	Material *lamp = new Lamp(1.1, Vector3D(1));



	/* ******* */
	/* Objects */
	/* ******* */
	// Create a heterogeneous list of objects of type shape
	// (some might be triangles, other spheres, plans, etc)
	objectsList = new std::vector<Shape*>;



	// Define and place a sphere
	Matrix4x4 sphereTransform1;
	sphereTransform1 = sphereTransform1.translate(Vector3D(-1.0, -0.5, 2 * std::sqrt(2.0)));
	Shape *s1 = new Sphere(0.25, sphereTransform1, blue_50);

	// Define and place a sphere
	Matrix4x4 sphereTransform2;
	sphereTransform2 = sphereTransform2.translate(Vector3D(1.0, 0.0, 6));
	Shape *s2 = new Sphere(1, sphereTransform2, green_50);

	// Define and place a sphere
	Matrix4x4 sphereTransform3;
	sphereTransform3 = sphereTransform3.translate(Vector3D(0.3, -0.75, 3.5));
	Shape *s3 = new Sphere(0.25, sphereTransform3, red_50);

	// Define and place a MOTH
	Matrix4x4 sphereTransform4;
	sphereTransform4 = sphereTransform4.translate(mothPosition);
	Moth *moth = new Moth(0.1, sphereTransform4, moth_50);
	
	

	Shape *ip = new InfinitePlane(Vector3D(0, -1.5, 0), Vector3D(0, 1, 0), white_50);
	Shape *backPlane = new InfinitePlane(Vector3D(0, 0, 7), Vector3D(0, 0, -1), blue_50);

	// Store the objects in the object list
	//objectsList->push_back(s1);
	//objectsList->push_back(s2);
	//objectsList->push_back(s3);

	mothList.push_back(moth);

	objectsList->push_back(moth);
	objectsList->push_back(ip);
	objectsList->push_back(backPlane);


	/* ****** */
	/* Lights */
	/* ****** */
	//

	lightSourceList = new std::vector<PointLightSource>;


	//Light Point Declaration
	PointLightSource l1 = PointLightSource(lampPosition1, Vector3D(5, 5, 5));
	Matrix4x4 sphereTransformL1;

	//Lamp Shape Declaration
	sphereTransformL1 = sphereTransform1.translate(lampPosition1);
	Shape *sL1 = new Sphere(0.1, sphereTransformL1, lamp);

	//Light Point Declaration
	PointLightSource l2 = PointLightSource(lampPosition2, Vector3D(5, 5, 5));
	Matrix4x4 sphereTransformL2;

	//Lamp Shape Declaration
	sphereTransformL2 = sphereTransform1.translate(lampPosition2);
	Shape *sL2 = new Sphere(0.1, sphereTransformL2, lamp);

	PointLightSource l3 = PointLightSource(Vector3D(3, 3, 3), Vector3D(15, 15, 15));
	Matrix4x4 sphereTransformL3;
	sphereTransformL3 = sphereTransform1.translate(Vector3D(3, 3, 3));
	Shape *sL3 = new Sphere(0.1, sphereTransformL3, lamp);

	PointLightSource l4 = PointLightSource(Vector3D(0, 7, 15), Vector3D(15, 15, 15));
	Matrix4x4 sphereTransformL4;
	sphereTransformL4 = sphereTransform1.translate(Vector3D(0, 7, 15));
	Shape *sL4 = new Sphere(0.1, sphereTransformL4, lamp);

	lightSourceList->push_back(l1);
	lightSourceList->push_back(l2);
	//lightSourceList->push_back(l3);
	//lightSourceList->push_back(l4);

	objectsList->push_back(sL1);
	objectsList->push_back(sL2);
	//objectsList->push_back(sL3);
	//objectsList->push_back(sL4);


}


void raytrace(Camera* &cam, Shader* &shader, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	unsigned int sizeBar = 40;

	size_t resX = film->getWidth();
	size_t resY = film->getHeight();

	// Main raytracing loop
	// Out-most loop invariant: we have rendered lin lines
	for (size_t lin = 0; lin<resY; lin++)
	{
		// Show progression
		if (lin % (resY / sizeBar) == 0)
			std::cout << ".";

		// Inner loop invariant: we have rendered col columns
		for (size_t col = 0; col<resX; col++)
		{
			// Compute the pixel position in NDC
			double x = (double)(col + 0.5) / resX;
			double y = (double)(lin + 0.5) / resY;

			// Generate the camera ray
			Ray cameraRay = cam->generateRay(x, y);

			// Compute ray color according to the used shader
			Vector3D pixelColor = shader->computeColor(cameraRay, *objectsList, *lightSourceList);

			// Store the pixel color
			film->setPixelValue(col, lin, pixelColor);
		}
	}
}

void mothLogic(Camera* &cam, Shader* &shader, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList, std::vector<Moth*> &mothList)
{
	

	Moth* m = mothList[0];
	double r = m->getRadius();
	Matrix4x4 MPosition;
	Vector3D dir;

	std::vector<Vector3D> pointList;

	pointList.push_back(Vector3D(mothPosition.x, mothPosition.y + r, mothPosition.z));
	pointList.push_back(Vector3D(mothPosition.x, mothPosition.y -r, mothPosition.z));
	pointList.push_back(Vector3D(r + mothPosition.x, mothPosition.y, mothPosition.z));
	pointList.push_back(Vector3D(mothPosition.x  -r, mothPosition.y, mothPosition.z));
	pointList.push_back(Vector3D(mothPosition.x, mothPosition.y, mothPosition.z + r));
	pointList.push_back(Vector3D(mothPosition.x, mothPosition.y, mothPosition.z -r));

	int directionPoint = m->mothDirection(pointList, *objectsList, *lightSourceList);

	//std::cout << directionPoint << std::endl;

	switch (directionPoint)
	{
	case 0:
		MPosition = m->getPosition();
		dir = Utils::moveUp(mothPosition);
		mothPosition = dir;
		MPosition = MPosition + Matrix4x4::translate(dir);
		m->setPosition(MPosition);
		break;

	case 1:
		MPosition = m->getPosition();
		dir = Utils::moveDown(mothPosition);
		mothPosition = dir;
		MPosition = MPosition + Matrix4x4::translate(dir);
		m->setPosition(MPosition);
		break;

	case 2:
		MPosition = m->getPosition();
		dir = Utils::moveRight(mothPosition);
		mothPosition = dir;
		MPosition = MPosition + Matrix4x4::translate(dir);
		m->setPosition(MPosition);
		break;

	case 3:
		MPosition = m->getPosition();
		dir = Utils::moveLeft(mothPosition);
		mothPosition = dir;
		MPosition = MPosition + Matrix4x4::translate(dir);
		m->setPosition(MPosition); mothPosition = Utils::moveLeft(mothPosition);
		break;

	case 4:
		MPosition = m->getPosition();
		dir = Utils::moveForward(mothPosition);
		mothPosition = dir;
		MPosition = MPosition + Matrix4x4::translate(dir);
		m->setPosition(MPosition);
		break;

	case 5:
		MPosition = m->getPosition();
		dir = Utils::moveBackwards(mothPosition);
		mothPosition = dir;
		MPosition = MPosition + Matrix4x4::translate(dir);
		m->setPosition(MPosition);
		break;

	}

	

}

void renderFrame(std::vector<Moth*> &mothList) {



	// Create an empty film
	Film *film;
	film = new Film(720, 576);


	// Declare the shader
	Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
	Vector3D intersectionColor(1, 0, 0);
	//Shader *shader = new IntersectionShader (intersectionColor, bgColor);
	//Shader *shader = new DepthShader(intersectionColor, 10, bgColor);
	//Shader *shader = new NormalShader(bgColor);
	Shader *shader = new DirectShader(Vector3D(0.4, 1, 0.4), 10, bgColor);
	//Shader *shader = new GlobalShader(bgColor, 2,10);


	// Declare pointers to all the variables which describe the scene
	Camera *cam;
	std::vector<Shape*> *objectsList;
	std::vector<PointLightSource> *lightSourceList;

	// Build the scene
	buildSceneSphere(cam, film, objectsList, lightSourceList, mothList);
	//buildSceneCornellBox(cam, film, objectsList, lightSourceList);


	// Launch some rays!
	raytrace(cam, shader, film, objectsList, lightSourceList);

	mothLogic(cam, shader, film, objectsList, lightSourceList, mothList);


	//First Lamp Logic
	if ((lampPosition1.x < (-1)) && lampDirection1)
		lampDirection1 = false;
	else if (lampPosition1.x > 1 && !lampDirection1)
		lampDirection1 = true;

	if(lampDirection1)
		lampPosition1 = Utils::moveLeft(lampPosition1);
	else
		lampPosition1 = Utils::moveRight(lampPosition1);


	//Second Lamp Logic
	if ((lampPosition2.y < (-1)) && lampDirection2)
		lampDirection2 = false;
	else if (lampPosition2.y > 1 && !lampDirection2)
		lampDirection2 = true;

	if (lampDirection2)
		lampPosition2 = Utils::moveForward(lampPosition2);
	else
		lampPosition2 = Utils::moveBackwards(lampPosition2);




	// Save the final result to file
	film->saveFrame();



}


int main()
{
	std::vector<Moth*> mothList;// = new std::vector<Moth*>;



	std::cout << "****************** Frame Rendering Started******************" << std::endl;

	for (int i = 0; i < 50; i++)
	{
		renderFrame(mothList);
	}

	std::cout << "****************** Frame Rendering Finished******************" << std::endl;

}