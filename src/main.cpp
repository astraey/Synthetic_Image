#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "shapes/sphere.h"
#include "cameras/ortographic.h"
#include "cameras/perspective.h"

void transformationsExercise()
{
    /* *************** */
    /* TRANSFORMATIONS */
    /* *************** */
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";

    std::cout << separatorStar << "Construct and visualize transforms" << separatorStar << std::endl;

    // Identity Matrix
    Matrix4x4 idMat; // By default, a matrix4x4 is initialized with the identity matrix
    std::cout << "The content of matrix idMat is: \n" << std::endl;
    std::cout << idMat << separator << std::endl;

    // Translation Matrix
    double deltaX, deltaY, deltaZ;
    deltaX = 1; deltaY = -2; deltaZ = 3;
    Vector3D delta(deltaX, deltaY, deltaZ);
    Matrix4x4 translationMat = Matrix4x4::translate(delta);
    std::cout << "The content of matrix translationMat is: \n" << std::endl;
    std::cout << translationMat << separator << std::endl;

    // Scale Matrix
	double sx, sy, sz;
	sx = 2; sy = 1; sz = -1;
	Vector3D sc(sx, sy, sz);
	Matrix4x4 scaleMatrix = Matrix4x4::scale(sc);
    std::cout << "The content of matrix scaleMatrix is: \n" << std::endl;
    std::cout << scaleMatrix << separator << std::endl;

    // Rotate around X Matrix
    double angleInDegrees = 60;
    double thetaInRadians = Utils::degreesToRadians(angleInDegrees);
	Vector3D rt(1, 0, 0);
	Matrix4x4 rotateXMatrix = Matrix4x4::rotate(thetaInRadians, rt);
    std::cout << "The content of matrix rotateXMatrix is: \n" << std::endl;
    std::cout << rotateXMatrix << separator << std::endl;

    // Rotate around an arbitrary axis Matrix
	double angleInDegrees2 = 30;
	double thetaInRadians2 = Utils::degreesToRadians(angleInDegrees2);
	Vector3D rt2(1, 1, 1);
	Matrix4x4 rotateArtitraryAxisMatrix = Matrix4x4::rotate(thetaInRadians2, rt2);
    std::cout << "The content of matrix rotateArtitraryAxisMatrix is: \n" << std::endl;
    std::cout << rotateArtitraryAxisMatrix << separator << std::endl;

    // Transposed and Inversion
    std::cout << separatorStar << "Inverting and Transposing a Matrix" << separatorStar << std::endl;
    Matrix4x4 translationMatTransposed;
    translationMat.transpose(translationMatTransposed);
    std::cout << "The transpose of matrix \n\n" << translationMat << "\n is \n\n"
              << translationMatTransposed << separator << std::endl;
    
	Matrix4x4 inverseTranslationMat;
	translationMat.inverse(inverseTranslationMat);
    std::cout << "The inverse of matrix \n\n" << translationMat << "\n is \n\n" << inverseTranslationMat << std::endl;

	Matrix4x4 MultiplicatedMatrix = translationMat.operator*(inverseTranslationMat);
	std::cout << "And their multiplication should thus give the identity matrix:\n\n" << MultiplicatedMatrix << std::endl;

    // Combine here some transforms, and visualize the result
    std::cout << separatorStar << "Combine transforms and visualize the result" << separatorStar << std::endl;

    Matrix4x4 scaleTranslate = scaleMatrix.operator*(translationMat);
    std::cout << "The content of matrix scaleTranslate is: \n" << std::endl;
    std::cout << scaleTranslate << separator << std::endl;

	Matrix4x4 translateScale = translationMat.operator*(scaleMatrix);
    std::cout << "The content of matrix translateScale is: \n" << std::endl;
    std::cout << translateScale << separator << std::endl;
}

void normalTransformExercise()
{
    std::string separator = "\n----------------------------------------------\n";

    // NOTE: points, vectors and normals are represented, in this framework, using
    //  the Vector3D class. It is up to the programmer to take care of whether the
    //  Vector3D is representing a point, a vector or a normal. For example, when
    //  applying a transform using a Matrix4x4, the programmer must choose which
    //  "applyTransform" method of the namespace Matrix4x4 should be used.

    Vector3D v(1, 1, 0);
    Vector3D n(-1.0/sqrt(2), 1.0/sqrt(2), 0);
    Matrix4x4 S = Matrix4x4::scale(Vector3D(2, 1, 1));

    std::cout << "Vector v = " << v << "\n" << std::endl;
    std::cout << "Normal n = " << n << "\n" << std::endl;
    std::cout << "Scale Matrix: \n" << std::endl;
    std::cout << S << std::endl;

	//We transfomr the vector v with the matrix S
    Vector3D vTransformed = S.transformVector(v);
    std::cout << "Vector v\' = " << vTransformed << "\n" << std::endl;

	//In this case, the transposed matrix is equal to the original
	Matrix4x4 STransposed;
	S.transpose(STransposed);
	std::cout << "STransposed \n" << STransposed << "\n" << std::endl;

	//We calculate the inverse of the previously Transposed Scale Matrix
	Matrix4x4 STransposedInverse;
	STransposed.inverse(STransposedInverse);
	std::cout << "STransposedInverse \n" << STransposedInverse << "\n" << std::endl;

	//We use the Transposed Inverted Matrix to transform the vector
	Vector3D nTransformed = STransposedInverse.transformVector(n);
	std::cout << "Vector n\' = " << nTransformed << "\n" << std::endl;

	//We calculate the dot product between the two vectors. If the result is 0, it means that 
	//the angle between the two vectors is indeed 90º (cos90 = 0), so we can confirm that the 
	//two vectors are perpendicular after scaling them 
	float result = dot(nTransformed, vTransformed);

	std::cout << "Result: " << result << "\n" << std::endl;

}

void paintingAnImageExercise()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);

	//We assign a color to every pixel based on its cordinates. The values for red 
	//and green never surpass 1. 
    for(unsigned int col = 0; col < resX; col++)
        {
            for(unsigned int row = 0; row < resY; row++)
            {
				float rCol = (col + 0.5) / resX;
				float gCol = (row + 0.5) / resY;
                Vector3D color(rCol, gCol, 0);
                film.setPixelValue(col, row, color);
            }
        }

    // Save the final result to file
    film.save();
	std::cout << "Image was successfully saved" << std::endl;
}

void filteringAnImageExercise()
{
	// Create two instances of the film class with the same resolution
	int resX, resY;
	resX = 512;
	resY = 512;
	Film f1(resX, resY);
	Film f2(resX, resY);

	// Create the original image
	//  Draw a circle centered at centerX, centerY (in pixels, image space)
	//   and with ray r (also in pixels)
	int centerX = resX / 2;
	int centerY = resY / 2;
	int r = std::min(centerX, centerY) / 2;
	for (int lin = 0; lin < resX; lin++)
	{
		for (int col = 0; col < resY; col++)
		{
			// Use the equation of the sphere to determine the pixel color
			if ((lin - centerX)*(lin - centerX) + (col - centerY)*(col - centerY) < r*r) {
				f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
			}
		}
	}



	int filterBorderWidth = 9;
	int filterXSize = 20;
	int filterYSize = 20;
	int totalNeighbours = filterXSize * filterYSize;
	int iterations = 5;

	for(int h = 0; h < iterations; h++)
	{
		for (int lin = 0; lin < resX; lin++)
		{
			for (int col = 0; col < resY; col++)
			{
				//We read the values of the neighbours of a pixel and fix its value depending on them. 
				Vector3D colorValue = Vector3D(0, 0, 0);
				unsigned targetX = lin - filterBorderWidth;
				unsigned targetY = col - filterBorderWidth;

				for (int i = 0; i < filterXSize; i++)
				{
					for (int j = 0; j < filterXSize; j++)
					{
						if (col + i < resX && lin + j < resY)
						{
							colorValue.operator+=(f1.getPixelValue(col + i, lin + j));
						}
					}
				}

				colorValue.operator/=(totalNeighbours);
				f2.setPixelValue(col, lin, colorValue);

			}
		}

		f1.clearData();

		//Copies the content of f2 in f1 to repeat the process in the next iteration;
		for (int lin = 0; lin < resX; lin++)
		{
			for (int col = 0; col < resY; col++)
			{

				f1.setPixelValue(col, lin, f2.getPixelValue(col,lin));
				
			}
		}


}

	//We save the final result
	f2.save();
	std::cout << "Filter was successfully applied to the image, which have been saved" << std::endl;

	
}

void completeSphereClassExercise()
{
    // Make your intersection tests here
    // (....)
}

void eqSolverExercise()
{
    EqSolver solver;
    rootValues roots;

    double A, B, C;

    // (...)

	bool hasRoots = true;
    //bool hasRoots = solver.rootQuadEq(A, B, C, roots);

    if(!hasRoots)
    {
        std::cout << "Equation has no real roots!" << std::endl;
    }
    else
    {
        // SHOW THE SOLUTIONS OF THE EQUATION
        // (...)
    }
}

void raytrace()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);

    /* ******************* */
    /* Orthographic Camera */
    /* ******************* */
    Matrix4x4 cameraToWorld; // By default, this gives an ID transform
                             // meaning that the camera space = world space
    OrtographicCamera camOrtho(cameraToWorld, film);

    /* ******************* */
    /* Perspective Camera */
    /* ******************* */
    double fovRadians = Utils::degreesToRadians(60);
    PerspectiveCamera camPersp(cameraToWorld, fovRadians, film);

    // Save the final result to file
    film.save();
}

int main()
{
    std::string separator = "\n----------------------------------------------\n";

    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;


    // ASSIGNMENT 1
    transformationsExercise();
    normalTransformExercise();
    paintingAnImageExercise();
    filteringAnImageExercise();

    // ASSIGNMENT 2
    //eqSolverExercise();
    //completeSphereClassExercise();
    //raytrace();
    std::cout << "\n\n" << std::endl;

    return 0;
}
