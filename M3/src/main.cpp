
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

Vector3D mothPosition1 = Vector3D(-0.75, -0.75, 4.5);

Vector3D mothPosition2 = Vector3D(0.75, 0.5, 3);

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

	// Define and place Moths
	Matrix4x4 sphereTransform4;
	sphereTransform4 = sphereTransform4.translate(mothPosition1);
	Moth *moth1 = new Moth(0.1, sphereTransform4, moth_50);

	Matrix4x4 sphereTransform5;
	sphereTransform5 = sphereTransform5.translate(mothPosition2);
	Moth *moth2 = new Moth(0.1, sphereTransform5, moth_50);
	
	

	Shape *ip = new InfinitePlane(Vector3D(0, -1.5, 0), Vector3D(0, 1, 0), white_50);
	Shape *backPlane = new InfinitePlane(Vector3D(0, 0, 7), Vector3D(0, 0, -1), blue_50);

	// Store the objects in the object list
	//objectsList->push_back(s1);
	//objectsList->push_back(s2);
	//objectsList->push_back(s3);

	mothList.push_back(moth1);
	mothList.push_back(moth2);

	objectsList->push_back(moth1);
	objectsList->push_back(moth2);
	objectsList->push_back(ip);
	objectsList->push_back(backPlane);


	/* ****** */
	/* Lights */
	/* ****** */
	//

	lightSourceList = new std::vector<PointLightSource>;


	//Light Point Declaration where intensity depends on position
	PointLightSource l1 = PointLightSource(lampPosition1, Vector3D(abs(lampPosition2.z), abs(lampPosition2.z), abs(lampPosition2.z)));
	Matrix4x4 sphereTransformL1;

	//Lamp Shape Declaration
	sphereTransformL1 = sphereTransform1.translate(lampPosition1);
	Shape *sL1 = new Sphere(0.1, sphereTransformL1, lamp);

	//Light Point Declaration where intensity depends on position
	PointLightSource l2 = PointLightSource(lampPosition2, Vector3D(abs(lampPosition1.x), abs(lampPosition1.x), abs(lampPosition1.x)));
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
	

	Moth* m1 = mothList[0];
	Moth* m2 = mothList[1];
	double r1 = m1->getRadius();
	double r2 = m2->getRadius();
	Matrix4x4 M1Position;
	Matrix4x4 M2Position;
	Vector3D dir1;
	Vector3D dir2;

	std::vector<Vector3D> pointList1;
	std::vector<Vector3D> pointList2;

	pointList1.push_back(Vector3D(mothPosition1.x, mothPosition1.y + r1, mothPosition1.z));
	pointList1.push_back(Vector3D(mothPosition1.x, mothPosition1.y -r1, mothPosition1.z));
	pointList1.push_back(Vector3D(r1 + mothPosition1.x, mothPosition1.y, mothPosition1.z));
	pointList1.push_back(Vector3D(mothPosition1.x  -r1, mothPosition1.y, mothPosition1.z));
	pointList1.push_back(Vector3D(mothPosition1.x, mothPosition1.y, mothPosition1.z + r1));
	pointList1.push_back(Vector3D(mothPosition1.x, mothPosition1.y, mothPosition1.z -r1));


	pointList2.push_back(Vector3D(mothPosition2.x, mothPosition2.y + r2, mothPosition2.z));
	pointList2.push_back(Vector3D(mothPosition2.x, mothPosition2.y - r2, mothPosition2.z));
	pointList2.push_back(Vector3D(r2 + mothPosition2.x, mothPosition2.y, mothPosition2.z));
	pointList2.push_back(Vector3D(mothPosition2.x - r2, mothPosition2.y, mothPosition2.z));
	pointList2.push_back(Vector3D(mothPosition2.x, mothPosition2.y, mothPosition2.z + r2));
	pointList2.push_back(Vector3D(mothPosition2.x, mothPosition2.y, mothPosition2.z - r2));

	int directionPoint1 = m1->mothDirection(pointList1, *objectsList, *lightSourceList);
	int directionPoint2 = m2->mothDirection(pointList2, *objectsList, *lightSourceList);

	//std::cout << directionPoint1 << "-"<< directionPoint2 << std::endl;

	switch (directionPoint1)
	{
	case 0:
		M1Position = m1->getPosition();
		dir1 = Utils::moveUp(mothPosition1);
		mothPosition1 = dir1;
		M1Position = M1Position + Matrix4x4::translate(dir1);
		m1->setPosition(M1Position);
		break;

	case 1:
		M1Position = m1->getPosition();
		dir1 = Utils::moveDown(mothPosition1);
		mothPosition1 = dir1;
		M1Position = M1Position + Matrix4x4::translate(dir1);
		m1->setPosition(M1Position);
		break;

	case 2:
		M1Position = m1->getPosition();
		dir1 = Utils::moveRight(mothPosition1);
		mothPosition1 = dir1;
		M1Position = M1Position + Matrix4x4::translate(dir1);
		m1->setPosition(M1Position);
		break;

	case 3:
		M1Position = m1->getPosition();
		dir1 = Utils::moveLeft(mothPosition1);
		mothPosition1 = dir1;
		M1Position = M1Position + Matrix4x4::translate(dir1);
		m1->setPosition(M1Position); mothPosition1 = Utils::moveLeft(mothPosition1);
		break;

	case 4:
		M1Position = m1->getPosition();
		dir1 = Utils::moveForward(mothPosition1);
		mothPosition1 = dir1;
		M1Position = M1Position + Matrix4x4::translate(dir1);
		m1->setPosition(M1Position);
		break;

	case 5:
		M1Position = m1->getPosition();
		dir1 = Utils::moveBackwards(mothPosition1);
		mothPosition1 = dir1;
		M1Position = M1Position + Matrix4x4::translate(dir1);
		m1->setPosition(M1Position);
		break;

	}




	switch (directionPoint2)
	{
	case 0:
		M2Position = m2->getPosition();
		dir2 = Utils::moveUp(mothPosition2);
		mothPosition2 = dir2;
		M2Position = M2Position + Matrix4x4::translate(dir2);
		m2->setPosition(M2Position);
		break;

	case 1:
		M2Position = m2->getPosition();
		dir2 = Utils::moveDown(mothPosition2);
		mothPosition2 = dir2;
		M2Position = M2Position + Matrix4x4::translate(dir2);
		m2->setPosition(M2Position);
		break;

	case 2:
		M2Position = m2->getPosition();
		dir2 = Utils::moveRight(mothPosition2);
		mothPosition2 = dir2;
		M2Position = M2Position + Matrix4x4::translate(dir2);
		m2->setPosition(M2Position);
		break;

	case 3:
		M2Position = m2->getPosition();
		dir2 = Utils::moveLeft(mothPosition2);
		mothPosition2 = dir2;
		M2Position = M2Position + Matrix4x4::translate(dir2);
		m2->setPosition(M2Position); mothPosition2 = Utils::moveLeft(mothPosition2);
		break;

	case 4:
		M2Position = m2->getPosition();
		dir2 = Utils::moveForward(mothPosition2);
		mothPosition2 = dir2;
		M2Position = M2Position + Matrix4x4::translate(dir2);
		m2->setPosition(M2Position);
		break;

	case 5:
		M2Position = m2->getPosition();
		dir2 = Utils::moveBackwards(mothPosition2);
		mothPosition2 = dir2;
		M2Position = M2Position + Matrix4x4::translate(dir2);
		m2->setPosition(M2Position);
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
	if ((lampPosition1.x < (-1.7)) && lampDirection1)
		lampDirection1 = false;
	else if (lampPosition1.x > 1.7 && !lampDirection1)
		lampDirection1 = true;

	if(lampDirection1)
		lampPosition1 = Utils::moveLeftLight(lampPosition1);
	else
		lampPosition1 = Utils::moveRightLight(lampPosition1);

	//std::cout << lampPosition2.z << "->  [2 - 5]" << std::endl;
	//Second Lamp Logic
	if (lampPosition2.z > 6  && lampDirection2)
		lampDirection2 = false;
	else if (lampPosition2.z < 1 && !lampDirection2)
		lampDirection2 = true;

	if (lampDirection2)
		lampPosition2 = Utils::moveForwardLight(lampPosition2);
	else
		lampPosition2 = Utils::moveBackwardsLight(lampPosition2);




	// Save the final result to file
	film->saveFrame();



}


int main()
{
	std::vector<Moth*> mothList;// = new std::vector<Moth*>;



	std::cout << "****************** Frame Rendering Started******************" << std::endl;

	for (int i = 0; i < 240; i++)
	{
		renderFrame(mothList);
	}

	std::cout << "****************** Frame Rendering Finished******************" << std::endl;

}