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
protected:
	int CountObj = 0;
	ScratchImage ImageSCR[1000];
	const wchar_t* ImageName[1000];
	vector<UINT> IndexCount;
protected:
	ComPtr<ID3D11Device> pDevice{ nullptr };
	ComPtr<ID3D11Buffer> pBuffer{ nullptr };
	ComPtr<ID3DBlob> pBlob{ nullptr };
	ComPtr<ID3D11VertexShader> pVertexShader{ nullptr };
	ComPtr<ID3D11PixelShader> pPixelShader{ nullptr };
	ComPtr<ID3D11InputLayout> pInputLayout{ nullptr };
protected:
	vector<ID3D11ShaderResourceView**> pSRVVec;
	vector<ID3D11SamplerState**> pSStateVec;
	vector<ID3D11Buffer**> pBufferVec;
	vector<ID3D11Buffer**> pBufferIndex;
	vector<ID3D11Buffer**> pBufferMatrix;
	vector<ID3D11VertexShader**> pVertexShaderVec;
	vector<ID3D11PixelShader**> pPixelShaderVec;
	vector<ID3D11InputLayout**> pInputLayoutVec;
};

class GraphicsLoader : public DXCollection
{
public:
	GraphicsLoader(float x, float y, float z, float width, float height, TypeObject typeobject, TypeBehavior typebehavior, const wchar_t* filename)
	{
		this->xLeft = (window.width / 2 - x) / (window.width / 2);
		this->xRight = (x + width - window.width / 2) / (window.width / 2);
		this->yTop = (window.height / 2 - y) / (window.height / 2);
		this->yBottom = (y + height - window.height / 2) / (window.height / 2);
		this->zBack = (z + width - window.width / 2) / (window.width / 2);
		this->zFront = (window.width / 2 - z) / (window.width / 2);

		if (typeobject = TypeObject::BOX2DTEX)
		{
			//если есть така€ текстура то мы ее не создаем снова
			CreateTexture2D(filename);
			
			//тут нужна проверка на наличие уже таких экземпл€ров если они есть то мы их не создаем снова
			CreateVertex2D();
			CreateIndex2D();
			SetShadersPSVS2D();
			CountObj++;
		}
	};
	~GraphicsLoader(){};
private:
	void CreateTexture2D(const wchar_t* filename)
	{
		ImageName[CountObj] = filename;
		ScratchImage Image;
		DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, ImageSCR[CountObj]);
		D3D11_TEXTURE2D_DESC TXDC{};
		TXDC.Width = ImageSCR[CountObj].GetImages()->width;
		TXDC.Height = ImageSCR[CountObj].GetImages()->height;
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
		SD.pSysMem = ImageSCR[CountObj].GetImages()->pixels;
		SD.SysMemPitch = ImageSCR[CountObj].GetImages()->rowPitch;

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

		Logg.Log(pDevice->CreateTexture2D(&TXDC, &SD, &pTex2D), "CreateTexture2D");
		Logg.Log(pDevice->CreateShaderResourceView(pTex2D.Get(), &SRVD, &pShaderResView), "CreateShaderResourceView");
		Logg.Log(pDevice->CreateSamplerState(&SDC, &pSampler), "CreateSamplerState");

		pSRVVec.push_back(pShaderResView.GetAddressOf());
		pSStateVec.push_back(pSampler.GetAddressOf());
	}
	void CreateVertex2D()
	{
		VEC3 arr[] =
		{
				{ -xLeft,-yBottom,zFront,  0.0f,1.0f },
				{ -xLeft,yTop,zFront,      0.0f,0.0f },
				{ xRight,yTop,zFront,      1.0f,0.0f },
				{ xRight,-yBottom,zFront,  1.0f,1.0f },
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

		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");

		pBufferVec.push_back(pBuffer.GetAddressOf());
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

		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");

		pBufferIndex.push_back(pBuffer.GetAddressOf());
		IndexCount.push_back(sizeof(Index)/ sizeof(Index[0]));
	}
	void SetShadersPSVS2D()
	{
		Logg.Log(D3DReadFileToBlob(L"TexPixelShader.cso", &pBlob), "D3DReadFileToBlobPS");
		Logg.Log(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader), "CreatePixelShader");
		pPixelShaderVec.push_back(pPixelShader.GetAddressOf());

		Logg.Log(D3DReadFileToBlob(L"TexVertexShader.cso", &pBlob), "D3DReadFileToBlobVS");
		Logg.Log(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader), "CreateVertexShader");
		pVertexShaderVec.push_back(pVertexShader.GetAddressOf());

		D3D11_INPUT_ELEMENT_DESC ELEMENT_DESC[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		Logg.Log(pDevice->CreateInputLayout(ELEMENT_DESC, sizeof(ELEMENT_DESC)/sizeof(ELEMENT_DESC[0]), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout), "CreateInputLayout");
		pInputLayoutVec.push_back(pInputLayout.GetAddressOf());
	}

private:
	float xLeft, xRight, yTop, yBottom, zBack, zFront;
	ComPtr<ID3D11Texture2D> pTex2D{ nullptr };
	ComPtr<ID3D11ShaderResourceView> pShaderResView{ nullptr };
	ComPtr<ID3D11SamplerState> pSampler{ nullptr };
};

//тут нужно бубет передавать данные
class GrphicsManipulation : public DXCollection
{
public:
	GrphicsManipulation() {};
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

		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");

		//Ё“ќ Ќ”∆Ќќ ѕ≈–≈ƒј¬ј“№
	}
	void ObjectMove(float OffsetX, float OffsetY, float OffsetZ, float AngleZ, const wchar_t* filename)
	{
			XMMATRIX matrx[] =
			{
				XMMatrixTranspose(
					XMMatrixTranslation(OffsetX,OffsetY,OffsetZ) *
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

			Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");

			//Ё“ќ Ќ”∆Ќќ ѕ≈–≈ƒј¬ј“№
	}
};


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
