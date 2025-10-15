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
	IDLE,
	TURN,
	JUMP,
	DEATH,
	ATTACK,
	RUN,

	LastEnum
};

class VEC3
{
public:
	VEC3() = default;
	VEC3(float x, float y,float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {};
private:
	float x{}, y{}, z{}, u{}, v{};
};

int GetWidthImage(string NameFile)
{
	ifstream file(NameFile, ios::binary | ios::in);
	int width = 0;
	char byte;
	int count = 0;
	int Pich = 4;
	while (file.get(byte))
	{
		int BiteNums = (int)byte;
		if (count >= 16)
		{
			if (BiteNums < 0)
				BiteNums = 256 + BiteNums;

			if (count != 19)
			{
				width += BiteNums * pow(16, Pich);
				Pich--;
			}
			else
			{
				width += BiteNums;
				Pich = 4;
				count = 0;
				break;
			}
		}
		count++;
	}
	return width;
}
int GetHeightImage(string NameFile)
{
	ifstream file(NameFile, ios::binary | ios::in);
	int width = 0;
	char byte;
	int count = 0;
	int Pich = 4;
	while (file.get(byte))
	{
		int BiteNums = (int)byte;
		if (count >= 20)
		{
			if (BiteNums < 0)
				BiteNums = 256 + BiteNums;

			if (count != 23)
			{
				width += BiteNums * pow(16, Pich);
				Pich--;
			}
			else
			{
				width += BiteNums;
				Pich = 4;
				count = 0;
				break;
			}
		}
		count++;
	}
	return width;
}
int GetSpeed(string NameFile)
{
	size_t digits = NameFile.find_first_of("1234567890");
	if (digits <= NameFile.size())
	return  atoi(NameFile.c_str() + digits);
}

pair<float, string> GetAnimation(StatusAnimate Status, string NameObj, vector<int>* TimeLineIt, vector<string>* TimeLineName, string Pattern)
{
	string NameStatus = "";
	string AddName = "";
	string NameCut = NameObj + ".png";
	int SpeedImage = 0;
	string path = "";
	int Width = 0;
	int Height = 0;
	
	for (int i = 0; i < animations.CollectionAnimation[Status - 1].size(); i++)
	{
		string Name = animations.CollectionAnimation[Status - 1][i];
		if (Name.find(NameObj) <= Name.size())
		{
			NameStatus = Name.replace(Name.find(NameCut), NameCut.length(), "");
			break;
		}
	}

	if (NameObj == "player")
		path = PLAYER ANI;

	else if (NameObj == "enemy")
		path = ENEMY ANI;
	
	if (Pattern == "no pattern" || Pattern == "")
	{ //базовый случай резкий переход

	if (Pattern == "")
		Pattern = "no pattern";

	TimeLineIt->resize(1);
	TimeLineName->resize(2);
	if (TimeLineIt[0][0] <= 0)
	{
		int Width  = GetWidthImage (path + NameStatus + NameObj + ".png");
		int Height = GetHeightImage(path + NameStatus + NameObj + ".png");
		TimeLineIt[0][0] = (Width / Height) * SpeedImage;
		TimeLineName[0][0] = NameStatus + NameObj;
	}
	SpeedImage = GetSpeed(NameStatus);
	TimeLineName[0][1] = Pattern;

	if (SpeedImage == 0)
		SpeedImage = 3;

	return pair<int, string>(SpeedImage, path + NameStatus + NameObj);
	}
	else
	{
		/// переменная которая понимает это первый раз проигрышь анимации или нет
		bool refrash = false;

		if ( TimeLineIt->size() == 1)
		{
			TimeLineIt->clear();
			TimeLineName->clear();
		}
		else if (TimeLineIt->size() == 2 && TimeLineName[0][2] != Pattern)
		{
			TimeLineIt->clear();
			TimeLineName->clear();
		}

			TimeLineName->resize(3);
			TimeLineIt->resize(2);

		/// запись и замена патерна на новый паттерн
		for (int i = 0; i < animations.PatternAnimation.size(); i++)
		{
			if (animations.PatternAnimation[i] == Pattern)
			{
				if (TimeLineName[0][0] == animations.PatternAnimation[i + 2] &&
					TimeLineName[0][1] == animations.PatternAnimation[i + 3]
					)
				{
					refrash = true;
				}

				TimeLineName[0][0] = animations.PatternAnimation[i+2];
				TimeLineName[0][1] = animations.PatternAnimation[i+3];
				break;
			}
		}
		
		if (TimeLineIt[0][0] <= 0 && TimeLineIt[0][1] <= 0 && refrash == false)
			{
				SpeedImage = GetSpeed(TimeLineName[0][0]);
				Width = GetWidthImage(path + TimeLineName[0][0] + ".png");
				Height = GetHeightImage(path + TimeLineName[0][0] + ".png");
				TimeLineIt[0][0] = (Width / Height) * SpeedImage;

				SpeedImage = GetSpeed(TimeLineName[0][1]);
				Width = GetWidthImage(path + TimeLineName[0][1] + ".png");
				Height = GetHeightImage(path + TimeLineName[0][1] + ".png");
				TimeLineIt[0][1] = (Width / Height) * SpeedImage;
			}
		else if (TimeLineIt[0][0] <= 0 && TimeLineIt[0][1] <= 0 && refrash == true)
			{
				SpeedImage = GetSpeed(TimeLineName[0][1]);
				Width = GetWidthImage(path + TimeLineName[0][1] + ".png");
				Height = GetHeightImage(path + TimeLineName[0][1] + ".png");
				TimeLineIt[0][1] = (Width / Height) * SpeedImage;
			}
		
		TimeLineName[0][2] = Pattern;

		if (SpeedImage == 0)
			SpeedImage = 3;

		if (TimeLineIt[0][0] > 0)
		{
		return pair<int, string>(SpeedImage, path + TimeLineName[0][0]);
		}
		else if (TimeLineIt[0][0] <= 0 && TimeLineIt[0][1] > 0)
		{
		return pair<int, string>(SpeedImage, path + TimeLineName[0][1]);
		}
	}
}

class BuildListBuffer
{
public:
	BuildListBuffer(float x, float y, float z, float width, float height,float ZAngle, float LookAtX, float LookAtY,int Iterator, StatusAnimate Status, int SpeedImage)
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
	 this->Status = Status;
	 this->SpeedImage = SpeedImage;
	};

	~BuildListBuffer(){};

	vector<VEC3> GetVectorList(TypeObject typeObject, vector<int>* TimeLineIt, vector<string>* TimeLineName, int Mirror)
	{
		 if (typeObject == TypeObject::BOX2DTEX && Status == StatusAnimate::DEFAULT)
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
		 else if (typeObject == TypeObject::BOX2DTEXSEEMLESS && Status == StatusAnimate::DEFAULT)
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
		 else if (typeObject == TypeObject::BOX2DTEXSEEMLESS_LMR && Status == StatusAnimate::DEFAULT)
	     {
		   Vectors.clear();
		   float StepX = 33; //ïðîèçâîëüíûé îòñòóï îò êðàÿ äî öåëüíîé ÷àñòè îáúåêòà
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

		 if (typeObject == TypeObject::BOX2DTEX && Status != StatusAnimate::DEFAULT)
		 {
			 int countNumber = TimeLineIt->size();
			 int Line = 0;

			 if (TimeLineIt[0][0] > 0)
			 {
				 TimeLineIt[0][0]--;
				 Line = TimeLineIt[0][0];
			 }
			 else if (TimeLineIt[0][0] <= 0 && countNumber == 2)
			 {
				 TimeLineIt[0][1]--;
				 Line = TimeLineIt[0][1];
			 }

			 int NextFrame = SpeedImage - (Line % SpeedImage);
			 int iterator = (((WidthImage / HeightImage)* SpeedImage) - Line - NextFrame) / SpeedImage;

			 if (HeightImage <= 0 || WidthImage <= 0)
			 {
				 int f = 1;
			 }


			 float PitchStart = (HeightImage * ((iterator * Mirror)-1)) / WidthImage;
			 float PitchEnd = (HeightImage * (iterator * Mirror)) / WidthImage;

			 Vectors =
			 {
				 { -xLeft,-yBottom,zFront,  PitchStart * Mirror,1.0f },
				 { -xLeft,yTop,zFront,      PitchStart * Mirror,0.0f },
				 { xRight,yTop,zFront,      PitchEnd * Mirror,0.0f },
				 { xRight,-yBottom,zFront,  PitchEnd * Mirror,1.0f },
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
		if (typeObject == TypeObject::BOX2DTEX || typeObject == TypeObject::BOX2DTEXSEEMLESS || typeObject == TypeObject::BOX2DTEXSEEMLESS_LMR)
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
	int   Iterator;
	float WidthImage, HeightImage;
	int SpeedImage;
	StatusAnimate Status;
};

