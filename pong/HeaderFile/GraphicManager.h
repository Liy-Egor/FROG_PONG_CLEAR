#pragma once
#include "BaseStructures.h"

using namespace DirectX;
using namespace Microsoft::WRL;

enum TypeObject
{
	BOX2DTEX,
	BOX2DTEXSEEMLESS,
	BOX2DTEXSEEMLESS_LMR,
	UI,
	UIX
};
class VEC3
{
public:
	VEC3() = default;
	VEC3(float x, float y,float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {};
private:
	float x{}, y{}, z{}, u{}, v{};
};

class BuildListBuffer
{
public:
	BuildListBuffer(float x, float y, float z, float width, float height,float ZAngle, float LookAtX, float LookAtY,int Iterator)
	{
	 this -> xLeft = (window.width / 2 - x) / (window.width / 2);
	 this -> xRight = (x + width - window.width / 2) / (window.width / 2);
	 this -> yTop = (window.height / 2 - y) / (window.height / 2);
	 this -> yBottom = (y + height - window.height / 2) / (window.height / 2);
	 this -> zBack = (z + width - window.width / 2) / (window.width / 2);
	 this -> zFront = (window.width / 2 - z) / (window.width / 2);
	 this -> ZAngle = ZAngle;
	 this -> CameraPosX = (LookAtX - window.width / 2) / (window.width / 2);
	 this -> CameraPosY = (LookAtY - window.height / 2) / (window.height / 2);
	 this -> WidthObject = width;
	 this -> HeightObject = height;
	 this -> Iterator = Iterator;
	};

	~BuildListBuffer(){};

	vector<VEC3> GetVectorList(TypeObject typeObject) 
	{
	   if (typeObject == TypeObject::BOX2DTEX)
		{
			Vectors =
			{
				{ -xLeft,-yBottom,zFront,  0.0f,1.0f },
				{ -xLeft,yTop,zFront,      0.0f,0.0f },
				{ xRight,yTop,zFront,      1.0f,0.0f },
				{ xRight,-yBottom,zFront,  1.0f,1.0f },
			};
			return Vectors;
		}
	   else if (typeObject == TypeObject::BOX2DTEXSEEMLESS)
	   {
		   float CountSeemX = WidthObject / WidthImage;
		   float CountSeemY = HeightObject / HeightImage;
		   Vectors =
		   {
			   { -xLeft,-yBottom,zFront,  0.0f,CountSeemY },
			   { -xLeft,yTop,zFront,      0.0f,0.0f },
			   { xRight,yTop,zFront,      CountSeemX,0.0f },
			   { xRight,-yBottom,zFront,  CountSeemX,CountSeemY },
		   };
		   return Vectors;
	   }
	   else if (typeObject == TypeObject::BOX2DTEXSEEMLESS_LMR)
	   {
		   float StepX = 33;
		   float CondertStepX = 1 - (window.width / 2 - StepX) / (window.width / 2);
		   float xRightStep1 = xRight - (xRight - CondertStepX);

		   float CondertStepX2 = WidthObject / WidthImage;
		   float xLeftStep2 = xLeft - CondertStepX;
		   float xRightStep2 = xRight - CondertStepX;

		   float CondertStepX3 = 1 - CondertStepX;
		   float xLeftStep3 = xRight - CondertStepX;
		   float xRightStep3 = xRight;


		   if (Iterator == 1)
		   {
		   Vectors =
		   {
			   { -xLeft,-yBottom,zFront,	   0.0f,1.0f },
			   { -xLeft,yTop,zFront,		   0.0f,0.0f },
			   { xRightStep1,yTop,zFront,      CondertStepX,0.0f },
			   { xRightStep1,-yBottom,zFront,  CondertStepX,1.0f },
			};
		   return Vectors;
		   }
		   else if (Iterator == 2)
		   {
			   Vectors =
			   {
				   { -xLeftStep2,-yBottom,zFront,       0.0f,1.0f },
				   { -xLeftStep2,yTop,zFront,           0.0f,0.0f },
				   { xRightStep2,yTop,zFront,      CondertStepX2,0.0f },
				   { xRightStep2,-yBottom,zFront,  CondertStepX2,1.0f },
			   };
			   return Vectors;
		   }
		   else if (Iterator == 3)
		   {
			   Vectors =
			   {
				   { -xLeftStep3,-yBottom,zFront,  0.0f,1.0f },
				   { -xLeftStep3,yTop,zFront,      0.0f,0.0f },
				   { xRightStep3,yTop,zFront,      CondertStepX3,0.0f },
				   { xRightStep3,-yBottom,zFront,  CondertStepX3,1.0f },
			   };
			   return Vectors;
		   }



	   }


	}

	vector<XMMATRIX> GetMatrix(TypeObject typeObject)
	{
		float PropScreen = (float)window.height / (float)window.width;
		if (typeObject == TypeObject::BOX2DTEX || 
			typeObject == TypeObject::BOX2DTEXSEEMLESS ||
			typeObject == TypeObject::BOX2DTEXSEEMLESS_LMR
			)
		{
			XMFLOAT3 CameraPos = { CameraPosX,-CameraPosY,-0.5f };
			XMFLOAT3 CameraTarget = { CameraPosX,-CameraPosY,0 };
			XMFLOAT3 Up = { 0,1,0 };
			Matrx =
			{
				XMMatrixTranspose(
					XMMatrixLookAtLH(XMLoadFloat3(&CameraPos),XMLoadFloat3(&CameraTarget),XMLoadFloat3(&Up)) *
					XMMatrixTranslation(0,0,0) *
					XMMatrixPerspectiveLH(1,1,0.9f,30.0f) *
					XMMatrixRotationZ(ZAngle)
				)
			};
			return Matrx;
		}

	}

	vector<unsigned short> GetIndex(TypeObject typeObject)
	{
		if (typeObject == TypeObject::BOX2DTEX || 
			typeObject == TypeObject::BOX2DTEXSEEMLESS ||
			typeObject == TypeObject::BOX2DTEXSEEMLESS_LMR
			)
		{
			Index =
			{
				{
				 0,1,2,
				 2,3,0,
				},
			};
			return Index;
		}


	}

	vector<D3D11_INPUT_ELEMENT_DESC> GetElementDesc(TypeObject typeObject)
	{
		
		if (typeObject == TypeObject::BOX2DTEX || 
			typeObject == TypeObject::BOX2DTEXSEEMLESS ||
			typeObject == TypeObject::BOX2DTEXSEEMLESS_LMR
			)
		{
			ELEMENT_DESC =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			return ELEMENT_DESC;
		}
	}

	void SetImageWH(float WidthImage, float HeightImage)
	{

		this->WidthImage = WidthImage;
		this->HeightImage = HeightImage;
	}
protected:
	vector<VEC3> Vectors;
	vector<XMMATRIX> Matrx;
	vector<unsigned short> Index;
	vector<D3D11_INPUT_ELEMENT_DESC> ELEMENT_DESC;
private:
	float xLeft, xRight, yBottom, yTop, zFront, zBack;
	float colorDelta;
	float ZAngle;
	float CameraPosX, CameraPosY;
	float WidthObject , HeightObject;
	int Iterator;
	float WidthImage, HeightImage;
};
