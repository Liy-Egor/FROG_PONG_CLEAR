#pragma once
#include "BaseStructures.h"
#include <DirectXMath.h>
using namespace DirectX;

enum TypeObject
{
	Box2D
};

class VEC3
{
public:
	VEC3() = default;
	VEC3(float x, float y,float z, float r, float g, float b) : x(x), y(y), z(z), r(r), g(g), b(b) {};
private:
	float x{}, y{}, z{}, r{}, g{}, b{};
};
vector<VEC3> vectors;

struct ConstBuffer
{
	struct
	{
		XMMATRIX matrx;
	}Translation;
};



class BuildListBuffer
{
public:
	BuildListBuffer(float x, float y, float z, float width, float height, float colorDelta)
	{
	 this -> xLeft = (window.width / 2 - x) / (window.width / 2);
	 this -> xRight = (x + width - window.width / 2) / (window.width / 2);
	 this -> yTop = (window.height / 2 - y) / (window.height / 2);
	 this -> yBottom = (y + height - window.height / 2) / (window.height / 2);
	 this -> zBack = (z + width - window.width / 2) / (window.width / 2);
	 this -> zFront = (window.width / 2 - z) / (window.width / 2);
	 this -> colorDelta = colorDelta;
	};
	~BuildListBuffer(){};

	vector<VEC3> GetVectorList(TypeObject typeObject)
	{
		if (typeObject == TypeObject::Box2D)
		{
			vectors =
			{
				{ -xLeft,-yBottom,zFront,  1 * colorDelta,0.3,      1 / colorDelta },
				{ -xLeft,yTop,zFront,      1 / colorDelta,			1 * colorDelta,0.3 },
				{ xRight,yTop,zFront,      1 / colorDelta,0.3,		1 / colorDelta },
				{ xRight,-yBottom,zFront,  0.3,1 / colorDelta,      1 * colorDelta },
			};
			return vectors;
		}




	}

private:
	float xLeft, xRight, yBottom, yTop, zFront, zBack;
	float colorDelta;
};