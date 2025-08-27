#pragma once
#include "BaseStructures.h"

using namespace DirectX;
using namespace Microsoft::WRL;

enum TypeObject
{
	BOX2DTEX,
	CAMERA,
	UI,
	UIX
};
enum TypeBehavior
{
	STATIC,
	DINAMIC
};

class VEC3
{
public:
	VEC3() = default;
	VEC3(float x, float y,float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {};
private:
	float x{}, y{}, z{}, u{}, v{};
};
struct DXCollection
{
public:
	int CountObj = 0;
	ScratchImage ImageSCR[1000];
	vector<const wchar_t*> ImageName;
	vector<const wchar_t*> ImageNameALL;
	vector<int> IdObject;
	vector<UINT> IndexCount;
	bool ThereImage = false;

	ComPtr<ID3D11Buffer> pBuffer{ nullptr };
	ComPtr<ID3DBlob> pBlobVS{ nullptr };
	ComPtr<ID3DBlob> pBlobPS{ nullptr };
	ComPtr<ID3D11InputLayout> pInputLayout{ nullptr };
	ComPtr<ID3D11ShaderResourceView> pShaderResView{ nullptr };
	ComPtr<ID3D11SamplerState> pSampler{ nullptr };
	ComPtr<ID3D11Texture2D> pTex2D{ nullptr };

	vector<ID3D11ShaderResourceView**> pSRVVec;
	vector<ID3D11SamplerState**> pSStateVec;
	vector<ID3D11Buffer**> pBufferVec;
	vector<ID3D11Buffer*> pBufferIndex;
	vector<ID3D11Buffer**> pBufferMatrix;
	vector<ID3D11Buffer**> pBufferCamera;

}DXColl;

//новая версия
class GraphicsLoader
{

public:
	GraphicsLoader(float x, float y, float z, float width, float height, TypeObject typeobject, const wchar_t* filename, int ID,ID3D11Device* dev)
	{
		DXColl.IdObject.push_back(ID);
		DXColl.ImageNameALL.push_back(filename);

		this->xLeft = (window.width / 2 - x) / (window.width / 2);
		this->xRight = (x + width - window.width / 2) / (window.width / 2);
		this->yTop = (window.height / 2 - y) / (window.height / 2);
		this->yBottom = (y + height - window.height / 2) / (window.height / 2);
		this->zBack = (z + width - window.width / 2) / (window.width / 2);
		this->zFront = (window.width / 2 - z) / (window.width / 2);

		this -> devs = dev;


		if (typeobject == TypeObject::BOX2DTEX)
		{			
			for (const wchar_t* var : DXColl.ImageName)
			{
				if (var == filename)
				{
					DXColl.ThereImage = true;
					break;
				}
			}

			if(DXColl.ThereImage == false)
			CreateTexture2D(filename);
			
			CreateMatrix2D();

			if (DXColl.CountObj == 0)
			{
			CreateVertex2D();
			CreateIndex2D();
			}
		}

		if (typeobject == TypeObject::CAMERA)
		{
			for (const wchar_t* var : DXColl.ImageName)
			{
				if (var == filename)
				{
					DXColl.ThereImage = true;
					break;
				}
			}

			if (DXColl.ThereImage == false)
				CreateTexture2D(filename);

			CreateMatrix2D();

			if (DXColl.CountObj == 0)
			{
				CreateVertex2D();
				CreateIndex2D();
				CreateCamera();
			}
		}

		DXColl.CountObj++;
	};
	~GraphicsLoader(){};


	


private:
	void CreateTexture2D(const wchar_t* filename)
	{
		DXColl.ThereImage = false;
		DXColl.ImageName.push_back(filename);
		ScratchImage Image;
		DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, DXColl.ImageSCR[DXColl.CountObj]);
		D3D11_TEXTURE2D_DESC TXDC{};
		TXDC.Width = DXColl.ImageSCR[DXColl.CountObj].GetImages()->width;
		TXDC.Height = DXColl.ImageSCR[DXColl.CountObj].GetImages()->height;
		TXDC.MipLevels = 1;
		TXDC.ArraySize = 1;
		TXDC.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		TXDC.SampleDesc.Count = 1;
		TXDC.SampleDesc.Quality = 0;
		TXDC.Usage = D3D11_USAGE_DEFAULT;
		TXDC.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TXDC.CPUAccessFlags = 0;
		TXDC.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = DXColl.ImageSCR[DXColl.CountObj].GetImages()->pixels;
		SD.SysMemPitch = DXColl.ImageSCR[DXColl.CountObj].GetImages()->rowPitch;

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVD{};
		SRVD.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		SRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVD.Texture2D.MostDetailedMip = 0;
		SRVD.Texture2D.MipLevels = 1;

		D3D11_SAMPLER_DESC SDC{};
		SDC.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SDC.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SDC.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SDC.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		Logg.Log(devs->CreateTexture2D(&TXDC, &SD, &DXColl.pTex2D), "CreateTexture2D");
		Logg.Log(devs->CreateShaderResourceView(DXColl.pTex2D.Get(), &SRVD, &DXColl.pShaderResView), "CreateShaderResourceView");
		Logg.Log(devs->CreateSamplerState(&SDC, &DXColl.pSampler), "CreateSamplerState");
		DXColl.pSRVVec.push_back(DXColl.pShaderResView.GetAddressOf());
		DXColl.pSStateVec.push_back(DXColl.pSampler.GetAddressOf());
	}
	void CreateVertex2D()
	{
		VEC3 arr[] =
		{
				{ -0.5,-0.5,1,  0.0f,1.0f },
				{ -0.5,0.5,1,      0.0f,0.0f },
				{ 0.5,0.5,1,      1.0f,0.0f },
				{ 0.5,-0.5,1,  1.0f,1.0f },
		};

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(arr);
		BD.StructureByteStride = sizeof(VEC3);
		BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BD.Usage = D3D11_USAGE_DEFAULT;
		BD.CPUAccessFlags = 0u;
		BD.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA SD;
		SD.pSysMem = arr;

		Logg.Log(devs->CreateBuffer(&BD, &SD, &DXColl.pBuffer), "CreateBuffer");
		DXColl.pBufferVec.push_back(DXColl.pBuffer.GetAddressOf());
	}
	void CreateIndex2D()
	{
		unsigned short Index[] =
		{
			{1},{0},{2},
			{2},{3},{0},
		};

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(Index);
		BD.StructureByteStride = sizeof(unsigned short);
		BD.BindFlags = D3D11_BIND_INDEX_BUFFER;
		BD.Usage = D3D11_USAGE_DEFAULT;
		BD.CPUAccessFlags = 0u;
		BD.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = Index;

		Logg.Log(devs->CreateBuffer(&BD, &SD, &DXColl.pBuffer), "CreateBuffer");
		DXColl.pBufferIndex.push_back(DXColl.pBuffer.Get());
		DXColl.IndexCount.push_back(sizeof(Index)/ sizeof(Index[0]));
	}
	void CreateMatrix2D()
	{
		XMMATRIX matrx[] =
		{
			XMMatrixTranspose
			(
			XMMatrixTranslation(0, 0, 0) *
			XMMatrixPerspectiveLH(1, 1, 0.9f, 30.0f) *
			XMMatrixRotationZ(0)
			)
		};
		
		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(matrx);
		BD.StructureByteStride = 0u;
		BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BD.Usage = D3D11_USAGE_DYNAMIC;
		BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BD.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = matrx;

		Logg.Log(devs->CreateBuffer(&BD, &SD, &DXColl.pBuffer), "CreateBuffer");
		DXColl.pBufferMatrix.push_back(DXColl.pBuffer.GetAddressOf());
	}
	void CreateCamera()
	{
		XMFLOAT3 CameraPos = { 0,0,-0.5f };
		XMFLOAT3 CameraTarget = { 0,0,0 };
		XMFLOAT3 Up = { 0,1,0 };

		XMMATRIX matrx[] =
		{
			XMMatrixTranspose(XMMatrixLookAtLH(XMLoadFloat3(&CameraPos),XMLoadFloat3(&CameraTarget),XMLoadFloat3(&Up)))
		};

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(matrx);
		BD.StructureByteStride = 0u;
		BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BD.Usage = D3D11_USAGE_DYNAMIC;
		BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BD.MiscFlags = 0u;


		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = matrx;

		Logg.Log(devs->CreateBuffer(&BD, &SD, &DXColl.pBuffer), "CreateBuffer");
		DXColl.pBufferCamera.push_back(DXColl.pBuffer.GetAddressOf());
	}

private:
	float xLeft, xRight, yTop, yBottom, zBack, zFront;
	ID3D11Device* devs;
};

class GraphicsManipulation : public DXCollection
{
public:
	GraphicsManipulation() {};
	void CameraMove(float TargetCameraX, float TargetCameraY)
	{
		float PropScreen = (float)window.height / (float)window.width;
		float CameraPosX = (TargetCameraX - window.width / 2) / (window.width / 2);
		float CameraPosY = (TargetCameraY - window.height / 2) / (window.height / 2);
		XMFLOAT3 CameraPos = { CameraPosX,-CameraPosY,-0.5f };
		XMFLOAT3 CameraTarget = { CameraPosX,-CameraPosY,0 };
		XMFLOAT3 Up = { 0,1,0 };
		XMMATRIX matrx[] =
		{
			XMMatrixTranspose(XMMatrixLookAtLH(XMLoadFloat3(&CameraPos),XMLoadFloat3(&CameraTarget),XMLoadFloat3(&Up)))
		};

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(matrx);
		BD.StructureByteStride = 0u;
		BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BD.Usage = D3D11_USAGE_DYNAMIC;
		BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BD.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = matrx;
	}
	void ObjectMove(float OffsetX, float OffsetY, float OffsetZ, float width, float height, float AngleZ, int ID)
	{
		float xLeft = (window.width / 2 - OffsetX) / (window.width / 2);
		float xRight = (OffsetX + width - window.width / 2) / (window.width / 2);
		float yTop = (window.height / 2 - OffsetY) / (window.height / 2);
		float yBottom = (OffsetY + height - window.height / 2) / (window.height / 2);
		float zBack = (OffsetZ + width - window.width / 2) / (window.width / 2);
		float zFront = (window.width / 2 - OffsetZ) / (window.width / 2);

		XMMATRIX matrx[] =
		{
			XMMatrixTranspose(
				XMMatrixScaling(xRight,yBottom,1) *
				XMMatrixTranslation(xLeft,yTop,1) *
				XMMatrixPerspectiveLH(1,1,0.9f,30.0f) *
				XMMatrixRotationZ(AngleZ)
			)
		};

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(matrx);
		BD.StructureByteStride = 0u;
		BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BD.Usage = D3D11_USAGE_DYNAMIC;
		BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BD.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = matrx;

		/*Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");

		pBufferMatrix[ID] = pBuffer.GetAddressOf();*/
	}
}GraphManipulation;


//старая версия
////////////////////////////////////////////////////////////////////////*******
vector<VEC3> Vectors;
vector<XMMATRIX> Matrx;
vector<unsigned short> Index;
vector<D3D11_INPUT_ELEMENT_DESC> ELEMENT_DESC;

ScratchImage DATASAVE2[1];
vector<D3D11_TEXTURE2D_DESC> TDESC;
vector<D3D11_SUBRESOURCE_DATA> SDATA;
vector<D3D11_SHADER_RESOURCE_VIEW_DESC> VDESC;
vector<D3D11_SAMPLER_DESC> SDESC;
void LoadImages(const wchar_t* filename)
{
	ScratchImage Image;
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
	}

	vector<XMMATRIX> GetMatrix(TypeObject typeObject)
	{
		float PropScreen = (float)window.height / (float)window.width;
		if (typeObject == TypeObject::BOX2DTEX)
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
		if (typeObject == TypeObject::BOX2DTEX)
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
		
		if (typeObject == TypeObject::BOX2DTEX)
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
