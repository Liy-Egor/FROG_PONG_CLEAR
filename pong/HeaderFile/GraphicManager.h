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

enum StatusAnimate
{
	DEFAULT,
	WALK,
	IDLE
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
	 this->Iterator = Iterator;
	};

	~BuildListBuffer(){};

	vector<VEC3> GetVectorList(TypeObject typeObject) 
	{
		if (status == StatusAnimate::DEFAULT)
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
		   Vectors.clear();
		   float StepX = 33; //произвольный отступ от края до цельной части объекта
		   float ConvertStepX =  1 - (window.width / 2 - StepX) / (window.width / 2);
		   float ConvertWimageX = 1 - (window.width / 2 - WidthImage) / (window.width / 2);
		   float xRStep1 = xLeft - ConvertStepX;
		   float xRStep4 = xRight - ConvertStepX;
		   
		   for (int i = 0; i < Iterator; i++)
		   {
			   if (i == 0)
			   {
				   /*  Vectors =
					 {
						 { -xLeft,-yBottom,zFront,0.0f,1.0f },
						 { -xLeft,yTop,zFront,0.0f,0.0f },
						 { -xRStep1,yTop,zFront,StepX / WidthImage,0.0f },
						 { -xRStep1,-yBottom,zFront,StepX / WidthImage,1.0f },
					 };
					 return Vectors;*/

				   Vectors.push_back({ -xLeft,-yBottom,zFront,0.0f,1.0f });
				   Vectors.push_back({ -xLeft,yTop,zFront,0.0f,0.0f });
				   Vectors.push_back({ -xRStep1,yTop,zFront,StepX / WidthImage,0.0f });
				   Vectors.push_back({ -xRStep1,-yBottom,zFront,StepX / WidthImage,1.0f });

			   }
			   else if (i > 0 && i < WidthObject / WidthImage + 1)
			   {
				   float offsetL2 = ConvertWimageX * (i - 1);
				   float offsetR2 = ConvertWimageX * ((WidthObject / WidthImage) - i);

				   float xLStep2 = xRStep1 - offsetL2;
				   float xRStep2 = xRStep4 - offsetR2;

				   /*Vectors =
				   {
					   { -xLStep2,-yBottom,zFront,     StepX / WidthImage,1.0f },
					   { -xLStep2,yTop,zFront,         StepX / WidthImage,0.0f },
					   { xRStep2,yTop,zFront,      1 - StepX / WidthImage,0.0f },
					   { xRStep2,-yBottom,zFront,  1 - StepX / WidthImage,1.0f },
				   };
				   return Vectors;*/

				   Vectors.push_back({ -xLStep2,-yBottom,zFront,     StepX / WidthImage,1.0f });
				   Vectors.push_back({ -xLStep2,yTop,zFront,         StepX / WidthImage,0.0f });
				   Vectors.push_back({ xRStep2,yTop,zFront,      1 - StepX / WidthImage,0.0f });
				   Vectors.push_back({ xRStep2,-yBottom,zFront,  1 - StepX / WidthImage,1.0f });
			   }
			   else if (i > 0 && i < WidthObject / WidthImage + ((WidthObject / WidthImage)))
			   {
				   float offsetR3 = ConvertWimageX * ((WidthObject / WidthImage) - (i - WidthObject / WidthImage));

				   float xLStep3 = xRStep4 - offsetR3;
				   float xRStep3 = xLStep3 + (ConvertStepX * 2);

				   float ImageWL = 0.5 - (StepX / (WidthImage - StepX * 2));
				   float ImageWR = 0.5 + (StepX / (WidthImage - StepX * 2));

				   /* Vectors =
					{
						{ xLStep3,-yBottom,zFront,     ImageWL,1.0f },
						{ xLStep3,yTop,zFront,         ImageWL,0.0f },
						{ xRStep3,yTop,zFront,		  ImageWR,0.0f },
						{ xRStep3,-yBottom,zFront,     ImageWR,1.0f },
					};
					return Vectors;*/

				   Vectors.push_back({ xLStep3,-yBottom,zFront,     ImageWL,1.0f });
				   Vectors.push_back({ xLStep3,yTop,zFront,         ImageWL,0.0f });
				   Vectors.push_back({ xRStep3,yTop,zFront,		  ImageWR,0.0f });
				   Vectors.push_back({ xRStep3,-yBottom,zFront,     ImageWR,1.0f });
			   }
			   else if (i == WidthObject / WidthImage + ((WidthObject / WidthImage)))
			   {
				   //Vectors =
				   //{
					  // { xRStep4,-yBottom,zFront,				 1 - StepX / WidthImage,1.0f },
					  // { xRStep4,yTop,zFront,					 1 - StepX / WidthImage,0.0f  },
					  // { xRight ,				yTop,zFront,	 1.0,0.0f },
					  // { xRight ,				-yBottom,zFront, 1.0,1.0f },
				   //};
				   //return Vectors;

				   Vectors.push_back({ xRStep4,-yBottom,zFront,				 1 - StepX / WidthImage,1.0f });
				   Vectors.push_back({ xRStep4,yTop,zFront,					 1 - StepX / WidthImage,0.0f });
				   Vectors.push_back({ xRight ,				yTop,zFront,	 1.0,0.0f });
				   Vectors.push_back({ xRight ,				-yBottom,zFront, 1.0,1.0f });
			   }
		   }

		   return Vectors;
	     }
		}
		else
		{
			//здесь нужно указать шаг для анимации
			if (TimeLineAnimation.size() > 1)
			{
				vector<int>::iterator it;
				it = TimeLineAnimation.begin();
				TimeLineAnimation.erase(it);
			}
			float ind = TimeLineAnimation[0];
			Vectors =
			{
				{ -xLeft,-yBottom,zFront,  0.0f,1.0f },
				{ -xLeft,yTop,zFront,      0.0f,0.0f },
				{ xRight,yTop,zFront,      1.0f,0.0f },
				{ xRight,-yBottom,zFront,  1.0f,1.0f },
			};


			return Vectors;
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
			Index.clear();
			for (int i = 0; i < Iterator; i++)
			{
				Index.push_back(0 + (i * 4));
				Index.push_back(1 + (i * 4));
				Index.push_back(2 + (i * 4));
				Index.push_back(2 + (i * 4));
				Index.push_back(3 + (i * 4));
				Index.push_back(0 + (i * 4));
			}
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

		if (status != StatusAnimate::DEFAULT)
		{
			TimeLineAnimation.push_back(WidthImage/70);
		}
	}

	//*****************************************
	string GetAnimation(StatusAnimate status)
	{
		this->status = status;
	}
	//*****************************************

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
	vector<int> TimeLineAnimation;
	StatusAnimate status = StatusAnimate::DEFAULT;
};
