#pragma once
#include "BaseStructures.h"


using namespace DirectX;
using namespace Microsoft::WRL;
enum TypeObject
{
	Box2D,
	Box2DTEX
};

class VEC3
{
public:
	VEC3() = default;
	VEC3(float x, float y,float z, float r, float g) : x(x), y(y), z(z), r(r), g(g) {};
private:
	float x{}, y{}, z{}, r{}, g{};
};


vector<VEC3> Vectors;
vector<XMMATRIX> Matrx;
vector<unsigned short> Index;
vector<D3D11_INPUT_ELEMENT_DESC> ELEMENT_DESC;

///////!!!!!!
ScratchImage DATASAVE2[1];
vector<D3D11_TEXTURE2D_DESC> TDESC;
vector<D3D11_SUBRESOURCE_DATA> SDATA;
vector<D3D11_SHADER_RESOURCE_VIEW_DESC> VDESC;
vector<D3D11_SAMPLER_DESC> SDESC;
void LoadImages(const wchar_t* filename)
{
	DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, DATASAVE2[0]);
	D3D11_TEXTURE2D_DESC TXDC{};
	TXDC.Width = DATASAVE2[0].GetImages()->width;
	TXDC.Height = DATASAVE2[0].GetImages()->height;
	TXDC.MipLevels = 1;
	TXDC.ArraySize = 1;
	TXDC.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	TXDC.SampleDesc.Count = 1;
	TXDC.SampleDesc.Quality = 0;
	TXDC.Usage = D3D11_USAGE_DEFAULT;
	TXDC.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TXDC.CPUAccessFlags = 0;
	TXDC.MiscFlags = 0;
	TDESC.push_back(TXDC);

	D3D11_SUBRESOURCE_DATA SD{};
	SD.pSysMem = DATASAVE2[0].GetImages()->pixels;
	SD.SysMemPitch = DATASAVE2[0].GetImages()->rowPitch;
	SDATA.push_back(SD);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVD{};
	SRVD.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVD.Texture2D.MostDetailedMip = 0;
	SRVD.Texture2D.MipLevels = 1;
	VDESC.push_back(SRVD);

	D3D11_SAMPLER_DESC SDC{};
	SDC.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SDC.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SDC.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SDC.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SDESC.push_back(SDC);
}
///////!!!!!!

class BuildListBuffer
{
public:
	BuildListBuffer(float x, float y, float z, float width, float height, float colorDelta,float ZAngle, float xPlayer, float yPlayer)
	{
	 this -> xLeft = (window.width / 2 - x) / (window.width / 2);
	 this -> xRight = (x + width - window.width / 2) / (window.width / 2);
	 this -> yTop = (window.height / 2 - y) / (window.height / 2);
	 this -> yBottom = (y + height - window.height / 2) / (window.height / 2);
	 this -> zBack = (z + width - window.width / 2) / (window.width / 2);
	 this -> zFront = (window.width / 2 - z) / (window.width / 2);

	 this -> CameraPosX = (xPlayer - window.width / 2) / (window.width / 2);
	 this -> CameraPosY = (yPlayer - window.height / 2) / (window.height / 2);

	 this -> colorDelta = colorDelta;
	 this -> ZAngle = ZAngle;
	 
	};
	~BuildListBuffer(){};

	vector<VEC3> GetVectorList(TypeObject typeObject)
	{
		if (typeObject == TypeObject::Box2D)
		{
			Vectors =
			{
				{ -xLeft,-yBottom,zFront,  1 * colorDelta,0.3 },
				{ -xLeft,yTop,zFront,      1 / colorDelta,			1 * colorDelta },
				{ xRight,yTop,zFront,      1 / colorDelta,0.3 },
				{ xRight,-yBottom,zFront,  0.3,1 / colorDelta },
			};
			return Vectors;
		}
		else if (typeObject == TypeObject::Box2DTEX)
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
	}

	vector<XMMATRIX> GetMatrix(TypeObject typeObject)
	{
		float PropScreen = (float)window.height / (float)window.width;
		if (typeObject == TypeObject::Box2D || typeObject == TypeObject::Box2DTEX)
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
		if (typeObject == TypeObject::Box2D || typeObject == TypeObject::Box2DTEX)
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
		if (typeObject == TypeObject::Box2D)
		{
			ELEMENT_DESC =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			return ELEMENT_DESC;
		}
		else if (typeObject == TypeObject::Box2DTEX)
		{
			ELEMENT_DESC =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			return ELEMENT_DESC;
		}
	}

	///////!!!!!!
	D3D11_TEXTURE2D_DESC GetTEXTURE2D_DESC()
	{
		return TDESC[0];
	}
	D3D11_SUBRESOURCE_DATA GetSUBRESOURCE_DATA()
	{
		return SDATA[0];
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC GetRESOURCE_VIEW_DESC()
	{
		return VDESC[0];
	}
	D3D11_SAMPLER_DESC GetSAMPLER_DESC()
	{
		return SDESC[0];
	}
	///////!!!!!!


private:
	float xLeft, xRight, yBottom, yTop, zFront, zBack;
	float colorDelta;
	float ZAngle;
	float CameraPosX;
	float CameraPosY;
};

