#pragma once

namespace zm {

	class SDF {
	public:

		//========================================================================
		//	2D Signed Distance Functions
		//========================================================================

		static float Circle(float x, float y, float r = 1.f);
		static float Square(float x, float y, float r = 1.f);
		static float Triangle(float x, float y, float r = 1.f);
		static float Pentagon(float x, float y, float r = 1.f);
		static float Star5(float x, float y, float r = 1.f);

		//========================================================================
		//	3D Signed Distance Functions
		//========================================================================

		static float Sphere(float x, float y, float z, float r = 1.f);
		static float Cube(float x, float y, float z, float r = 1.f);
	};
}