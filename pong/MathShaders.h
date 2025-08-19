#pragma once
#include "BaseStructures.h"

enum TypeBuffer
{
	Index,
	Constant,
	Vertex,
};

enum TypeObject
{
	Box2D,
	Triangle2D,
	Box3D
};

class VEC3
{
public:
	VEC3() = default;
	VEC3(float x, float y, float z) : x(x), y(y), z(z) {}
	VEC3(XMMATRIX) {};
private:
	float x{}, y{}, z{};
};
