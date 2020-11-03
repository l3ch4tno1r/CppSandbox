#include <iostream>

#include "LCN_Math/Source/_Geometry/Vector.h"

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl
#define DISPLAY(X) std::cout << #X << " = " << X << std::endl;

int main()
{
	{
		Vector3Df vec1 = { 1, 2, 3 };
		Vector3Df vec2 = { 4, 5, 6 };
		Vector3Df vec3 = vec1 + vec2;

		//vec3 = vec1 + vec2;

		std::cout << vec3 << std::endl;
		std::cout << (vec1 | vec2) << std::endl;

		SEPARATOR("Vector 2D");

		Vector2Df pvec = { 1, 2 };

		std::cout << "x = " << pvec.x() << std::endl;
		std::cout << "y = " << pvec.y() << std::endl;
		//std::cout << "w = " << pvec.w() << std::endl;
		//std::cout << "z = " << pvec.z() << std::endl;

		SEPARATOR("Homogeneous vector 3D");

		HVector3Df hvec1 = { 4, 2, 3, 1 };

		std::cout << "x = " << hvec1.x() << std::endl;
		std::cout << "y = " << hvec1.y() << std::endl;
		std::cout << "z = " << hvec1.z() << std::endl;
		std::cout << "w = " << hvec1.w() << std::endl;
		std::cout << "Lines = " << hvec1.Line() << std::endl;

		SEPARATOR("Homogeneous vector 2D");

		HVector2Df hpvec = { 3, 4, 1 };

		std::cout << "x = " << hpvec.x() << std::endl;
		std::cout << "y = " << hpvec.y() << std::endl;
		std::cout << "w = " << hpvec.w() << std::endl;

		SEPARATOR(1);

		std::cout << std::boolalpha;

		DISPLAY(Vector2Df::cm_X_Accessible);
		DISPLAY(Vector2Df::cm_Y_Accessible);
		DISPLAY(Vector2Df::cm_Z_Accessible);
		DISPLAY(Vector2Df::cm_W_Accessible);

		SEPARATOR(2);

		DISPLAY(HVector2Df::cm_X_Accessible);
		DISPLAY(HVector2Df::cm_Y_Accessible);
		DISPLAY(HVector2Df::cm_Z_Accessible);
		DISPLAY(HVector2Df::cm_W_Accessible);

		SEPARATOR(3);

		DISPLAY(Vector3Df::cm_X_Accessible);
		DISPLAY(Vector3Df::cm_Y_Accessible);
		DISPLAY(Vector3Df::cm_Z_Accessible);
		DISPLAY(Vector3Df::cm_W_Accessible);

		SEPARATOR(4);

		DISPLAY(HVector3Df::cm_X_Accessible);
		DISPLAY(HVector3Df::cm_Y_Accessible);
		DISPLAY(HVector3Df::cm_Z_Accessible);
		DISPLAY(HVector3Df::cm_W_Accessible);
	}

	SEPARATOR("Cross product");
	{
		Vector3Df vec1 = { 1, 2, 3 };
		Vector3Df vec2 = { 4, 5, 6 };

		Vector3Df cprod = vec1 ^ vec2;

		std::cout << cprod << std::endl;

		HVector3Df hvec1 = { 1, 2, 3, 1 };
		HVector3Df hvec2 = { 4, 5, 6, 1 };

		//HVector3Df hcprod = hvec1 ^ hvec2; // Will fail on purpose
	}

	SEPARATOR("Homogeneous");
	{
		Vector3Df vec1 = { 1, 2, 3 };
		auto hvec1 = vec1.Homogeneous(0);

		std::cout << hvec1 << std::endl;
	}

	std::cin.get();
}