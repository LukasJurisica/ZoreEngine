#pragma once

#include <cmath>
#include "math/MathUtils.hpp"

namespace fnl {

	//========================================================================
	//	Enum Definitions
	//========================================================================

	enum class NoiseType {
		OpenSimplex2, OpenSimplex2S, Cellular, Perlin, ValueCubic, Value
	};

	enum class RotationType3D {
		None, ImproveXYPlanes, ImproveXZPlanes
	};

	enum class TransformType3D {
		None, ImproveXYPlanes, ImproveXZPlanes, DefaultOpenSimplex2
	};

	enum class DomainWarpType {
		OpenSimplex2, OpenSimplex2Reduced, BasicGrid
	};

	enum class FractalType {
		None, FBm, Ridged, PingPong, DomainWarpProgressive, DomainWarpIndependent
	};

	enum class CellularDistanceFunction {
		Euclidean, EuclideanSq, Manhattan, Hybrid
	};

	enum class CellularReturnType {
		CellValue, Distance, Distance2, Distance2Add, Distance2Sub, Distance2Mul, Distance2Div
	};

	class FastNoiseLite {
	public:
		FastNoiseLite(int seed = 1337);
		~FastNoiseLite() = default;

		void SetSeed(int seed);
		void SetFrequency(float frequency);
		void SetNoiseType(NoiseType noiseType);

		void SetFractalType(FractalType fractalType);
		void SetFractalOctaves(int octaves);
		void SetFractalLacunarity(float lacunarity);
		void SetFractalPersistence(float persistence);
		void SetFractalWeightedStrength(float weightedStrength);
		void SetFractalPingPongStrength(float pingPongStrength);

		void SetRotationType3D(RotationType3D rotationType3D);
		void SetDomainWarpType(DomainWarpType domainWarpType);
		void SetDomainWarpAmp(float domainWarpAmp);

		void SetCellularDistanceFunction(CellularDistanceFunction cellularDistanceFunction);
		void SetCellularReturnType(CellularReturnType cellularReturnType);
		void SetCellularJitter(float cellularJitter);

		float GetNoise(float x, float y);
		float GetNoise(float x, float y, float z);
		void DomainWarp(float& x, float& y);
		void DomainWarp(float& x, float& y, float& z);

	private:
		float GenNoiseSingle(int seed, float x, float y);
		float GenNoiseSingle(int seed, float x, float y, float z);
		float SinglePerlin(int seed, float x, float y);
		float SinglePerlin(int seed, float x, float y, float z);
		float SingleValue(int seed, float x, float y);
		float SingleValue(int seed, float x, float y, float z);
		float SingleValueCubic(int seed, float x, float y);
		float SingleValueCubic(int seed, float x, float y, float z);
		float SingleSimplex(int seed, float x, float y);
		float SingleOpenSimplex2(int seed, float x, float y, float z);
		float SingleOpenSimplex2S(int seed, float x, float y);
		float SingleOpenSimplex2S(int seed, float x, float y, float z);
		float SingleCellular(int seed, float x, float y);
		float SingleCellular(int seed, float x, float y, float z);

		float GenFractalFBm(float x, float y);
		float GenFractalFBm(float x, float y, float z);
		float GenFractalRidged(float x, float y);
		float GenFractalRidged(float x, float y, float z);
		float GenFractalPingPong(float x, float y);
		float GenFractalPingPong(float x, float y, float z);

		void DoSingleDomainWarp(int seed, float amp, float freq, float x, float y, float& xr, float& yr);
		void DoSingleDomainWarp(int seed, float amp, float freq, float x, float y, float z, float& xr, float& yr, float& zr);
		void DomainWarpSingle(float& x, float& y);
		void DomainWarpSingle(float& x, float& y, float& z);
		void DomainWarpFractalProgressive(float& x, float& y);
		void DomainWarpFractalProgressive(float& x, float& y, float& z);
		void DomainWarpFractalIndependent(float& x, float& y);
		void DomainWarpFractalIndependent(float& x, float& y, float& z);
		void SingleDomainWarpBasicGrid(int seed, float warpAmp, float frequency, float x, float y, float& xr, float& yr);
		void SingleDomainWarpBasicGrid(int seed, float warpAmp, float frequency, float x, float y, float z, float& xr, float& yr, float& zr);
		void SingleDomainWarpSimplexGradient(int seed, float warpAmp, float frequency, float x, float y, float& xr, float& yr, bool outGradOnly);
		void SingleDomainWarpOpenSimplex2Gradient(int seed, float warpAmp, float frequency, float x, float y, float z, float& xr, float& yr, float& zr, bool outGradOnly);

		void TransformNoiseCoordinate(float& x, float& y);
		void TransformNoiseCoordinate(float& x, float& y, float& z);
		void TransformDomainWarpCoordinate(float& x, float& y);
		void TransformDomainWarpCoordinate(float& x, float& y, float& z);

		void UpdateFractalBounding();
		void UpdateTransformType3D();
		void UpdateWarpTransformType3D();

		static int Hash(int seed, int xPrimed, int yPrimed);
		static int Hash(int seed, int xPrimed, int yPrimed, int zPrimed);
		static float ValCoord(int seed, int xPrimed, int yPrimed);
		static float ValCoord(int seed, int xPrimed, int yPrimed, int zPrimed);
		float GradCoord(int seed, int xPrimed, int yPrimed, float xd, float yd);
		float GradCoord(int seed, int xPrimed, int yPrimed, int zPrimed, float xd, float yd, float zd);
		void GradCoordOut(int seed, int xPrimed, int yPrimed, float& xo, float& yo);
		void GradCoordOut(int seed, int xPrimed, int yPrimed, int zPrimed, float& xo, float& yo, float& zo);
		void GradCoordDual(int seed, int xPrimed, int yPrimed, float xd, float yd, float& xo, float& yo);
		void GradCoordDual(int seed, int xPrimed, int yPrimed, int zPrimed, float xd, float yd, float zd, float& xo, float& yo, float& zo);

	private:
		int mSeed;
		float mFrequency;
		NoiseType mNoiseType;
		RotationType3D mRotationType3D;
		TransformType3D mTransformType3D;

		FractalType mFractalType;
		int mOctaves;
		float mLacunarity;
		float mPersistence;
		float mWeightedStrength;
		float mPingPongStrength;
		float mFractalBounding;

		CellularDistanceFunction mCellularDistanceFunction;
		CellularReturnType mCellularReturnType;
		float mCellularJitterModifier;

		DomainWarpType mDomainWarpType;
		TransformType3D mWarpTransformType3D;
		float mDomainWarpAmp;

		static const float Gradients2D[];
		static const float Gradients3D[];
		static const float RandVecs2D[];
		static const float RandVecs3D[];
	};
}