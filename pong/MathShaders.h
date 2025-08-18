#pragma once

class VEC2
{
public:
	VEC2() = default;
	VEC2(float x, float y, float r, float g, float b) : x(x), y(y), r(r), g(g), b(b) {};
private:
	float x{}, y{}, r{}, g{}, b{};
};

class VEC3
{
public:
	VEC3() = default;
	VEC3(float x, float y,float z) : x(x), y(y), z(z) {}
private:
	float x{}, y{}, z{};

};

struct ConstBuffer
{
	struct
	{
		float matrx[4][4];
	}Translation;
};

