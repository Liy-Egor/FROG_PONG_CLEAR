#pragma once
#include "GraphicManager.h"
//ÒÓÒÎÐÈÀËÛ
//https://www.youtube.com/@PardCode
//https://www.rastertek.com/tutdx11win10.html
//https://learn.microsoft.com/ru-ru/windows/win32/api/d3d11/
//https://www.youtube.com/watch?v=YFrCrasocX8&list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&index=19
//https://github.com/planetchili/hw3d/blob/T27-End/hw3d/Surface.h


class GraphicEngine
{
public:
	GraphicEngine()
	{
		DXGI_SWAP_CHAIN_DESC SWdesc{};
		SWdesc.BufferDesc.Width = 0;
		SWdesc.BufferDesc.Height = 0;
		SWdesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		SWdesc.BufferDesc.RefreshRate.Numerator = 0;
		SWdesc.BufferDesc.RefreshRate.Denominator = 0;
		SWdesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SWdesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SWdesc.SampleDesc.Count = 1;
		SWdesc.SampleDesc.Quality = 0;
		SWdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SWdesc.BufferCount = 2;
		SWdesc.OutputWindow = win.GetHWND();
		SWdesc.Windowed = TRUE;
		SWdesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SWdesc.Flags = 0;

		Logg.Log(hr = D3D11CreateDeviceAndSwapChain
		(nullptr,D3D_DRIVER_TYPE_HARDWARE,nullptr,0,nullptr,0,D3D11_SDK_VERSION,&SWdesc,&pGISwapChain,&pDevice,nullptr,&pDeviceContext), "CreateDevice");

		Logg.Log(pGISwapChain->GetBuffer(0, IID_PPV_ARGS(&pResource)), "GetBuffer");
		Logg.Log(pDevice->CreateRenderTargetView(pResource.Get(), nullptr, &pRenderTargetView), "CreateRenderTargetView");
	}
	~GraphicEngine()
	{


	}

	void RenderClearBuffer(float red, float green, float blue);
	void Present(bool vSync);
	void DrawObject(float x, float y, float z, float width, float height,float ZAngle, TypeObject typeOBJ, const wchar_t* filename);
	void SetCamera(float LookAtX, float LookAtY)
	{
		this->LookAtX = LookAtX;
		this->LookAtY = LookAtY;
	};
private:

	vector<ID3D11ShaderResourceView**> pSRV;
	vector<ID3D11SamplerState**> pSST;
	ScratchImage TexList[500];

	vector<const wchar_t*> TexNameList;
	void CreateTextureBuffer(const wchar_t* filename)
	{
		int IndexTex = 0;
		if (TexNameList.size() == 0)
		{
		DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, TexList[TexNameList.size()]);

		D3D11_TEXTURE2D_DESC TXDC{};
		TXDC.Width = TexList[TexNameList.size()].GetImages()->width;
		TXDC.Height = TexList[TexNameList.size()].GetImages()->height;
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
		SD.pSysMem = TexList[TexNameList.size()].GetImages()->pixels;
		SD.SysMemPitch = TexList[TexNameList.size()].GetImages()->rowPitch;

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

		pDevice->CreateTexture2D(&TXDC, &SD, &pTex2D);
		pDevice->CreateShaderResourceView(pTex2D.Get(), &SRVD, &pShaderResView);
		pDevice->CreateSamplerState(&SDC, &pSampler);

		ID3D11ShaderResourceView** pSrv = new ID3D11ShaderResourceView*;
		ID3D11SamplerState** pSs = new ID3D11SamplerState*;

		pDevice->CreateTexture2D(&TXDC, &SD, &pTex2D);
		pDevice->CreateShaderResourceView(pTex2D.Get(), &SRVD, pSrv);
		pDevice->CreateSamplerState(&SDC, pSs);

		pSRV.push_back(pSrv);
		pSST.push_back(pSs);
		IndexTex = 0;
		TexNameList.push_back(filename);
		}
		else
		{
		for (int i = 0; i < TexNameList.size(); i++)
		{
			if (TexNameList[i] == filename)
			{
				IndexTex = i;
				break;
			}
			else if (TexNameList[i] != filename && i == TexNameList.size() - 1)
			{
				DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, TexList[TexNameList.size()]);

				D3D11_TEXTURE2D_DESC TXDC{};
				TXDC.Width = TexList[TexNameList.size()].GetImages()->width;
				TXDC.Height = TexList[TexNameList.size()].GetImages()->height;
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
				SD.pSysMem = TexList[TexNameList.size()].GetImages()->pixels;
				SD.SysMemPitch = TexList[TexNameList.size()].GetImages()->rowPitch;

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

				ID3D11ShaderResourceView** pSrv = new ID3D11ShaderResourceView*;
				ID3D11SamplerState** pSs = new ID3D11SamplerState*;

				pDevice->CreateTexture2D(&TXDC, &SD, &pTex2D);
				pDevice->CreateShaderResourceView(pTex2D.Get(), &SRVD, pSrv);
				pDevice->CreateSamplerState(&SDC, pSs);

				pSRV.push_back(pSrv);
				pSST.push_back(pSs);
				IndexTex = TexNameList.size();
				TexNameList.push_back(filename);
			}
		}
		}

		pDeviceContext->PSSetShaderResources(0u, 1u, pSRV[IndexTex]);
		pDeviceContext->PSSetSamplers(0, 1, pSST[IndexTex]);
	}
	void CreateInputLayer(vector<D3D11_INPUT_ELEMENT_DESC> ElementDesc)
	{
		D3D11_INPUT_ELEMENT_DESC* arr = new D3D11_INPUT_ELEMENT_DESC[ElementDesc.size()];
		for (int i = 0; i < ElementDesc.size(); ++i) {
			arr[i] = ElementDesc[i];
		}
		Logg.Log(pDevice->CreateInputLayout(arr, ElementDesc.size(), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
		pDeviceContext->IASetInputLayout(pInputLayout.Get());
		delete[](arr);
	}
	UINT CreateIndexBuff(vector<unsigned short> Index)
	{
		unsigned short* arr = new unsigned short[Index.size()];
		for (int i = 0; i < Index.size(); ++i) {
			arr[i] = Index[i];
		}

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(Index[0]) * Index.size();
		BD.StructureByteStride = sizeof(unsigned short);
		BD.BindFlags = D3D11_BIND_INDEX_BUFFER;
		BD.Usage = D3D11_USAGE_DEFAULT;
		BD.CPUAccessFlags = 0u;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = arr;
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pIndexBuffer), "CreateBuffer");

		pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(),DXGI_FORMAT_R16_UINT,0u);

		delete[](arr);
		return Index.size();
	}
	void CreateMatrixBuffer(vector<XMMATRIX> matrix)
	{
		XMMATRIX* arr = new XMMATRIX[matrix.size()];
		for (int i = 0; i < matrix.size(); ++i) 
		{
			arr[i] = matrix[i];
		}
		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(matrix[0]) * matrix.size();
		BD.StructureByteStride = 0u;
		BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BD.Usage = D3D11_USAGE_DYNAMIC;
		BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = arr;
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pConstBuffer), "CreateBuffer");
		pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
		delete[](arr);
	}
	void CreateVectorBuff(vector<VEC3> List)
	{
		VEC3* arr = new VEC3[List.size()];
		for (int i = 0; i < List.size(); ++i) {
			arr[i] = List[i];
		}

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(List[0]) * List.size();
		BD.StructureByteStride = sizeof(VEC3);
		BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BD.Usage = D3D11_USAGE_DEFAULT;
		BD.CPUAccessFlags = 0u;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD;
		SD.pSysMem = arr;
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");
		const UINT stride = sizeof(VEC3);
		const UINT offset = 0u;
		pDeviceContext->IASetVertexBuffers(0u, 1u, pBuffer.GetAddressOf(), &stride, &offset);
		delete[](arr);
	} 
	void SetShadersVSPS(TypeObject typeOBJ)
	{
		const wchar_t* VSNameFilecso = L"";
		const wchar_t* PSNameFilecso = L"";
		if (TypeObject::BOX2DTEX == typeOBJ)
		{
			VSNameFilecso = L"TexVertexShader.cso";
			PSNameFilecso = L"TexPixelShader.cso";
		}

		Logg.Log(D3DReadFileToBlob(VSNameFilecso, &pBlobVS), "D3DReadFileToBlobVS");
		Logg.Log(D3DReadFileToBlob(PSNameFilecso, &pBlobPS), "D3DReadFileToBlobPS");
		Logg.Log(pDevice->CreateVertexShader(pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), nullptr, &pVertexShader), "CreateVertexShader");
		pDeviceContext->VSSetShader(pVertexShader.Get(), 0, 0);
		Logg.Log(pDevice->CreatePixelShader(pBlobPS->GetBufferPointer(), pBlobPS->GetBufferSize(), nullptr, &pPixelShader), "CreatePixelShader");
		pDeviceContext->PSSetShader(pPixelShader.Get(), 0, 0);
	}
	void SetViewports(float MinDepth, float MaxDepth,float TopLeftX, float TopLeftY,UINT countViewports)
	{
		D3D11_VIEWPORT VP{};
		VP.Width = window.width;
		VP.Height = window.height;
		VP.MinDepth = MinDepth;
		VP.MaxDepth = MaxDepth;
		VP.TopLeftX = TopLeftX;
		VP.TopLeftY = TopLeftY;
		pDeviceContext->RSSetViewports(countViewports, &VP);
	}
	void RenderDrawIndex(D3D_PRIMITIVE_TOPOLOGY MetodDraw, UINT IndexCount)
	{
		pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);
		pDeviceContext->IASetPrimitiveTopology(MetodDraw);
		pDeviceContext->DrawIndexed(IndexCount, 0u, 0u);
	}
	
private:
	GraphicEngine(const GraphicEngine&) = delete;
	GraphicEngine(GraphicEngine&) = delete;
	GraphicEngine& operator = (const GraphicEngine) = delete;
	GraphicEngine& operator = (GraphicEngine&) = delete;
private:
	HRESULT hr;
	ComPtr<ID3D11Device> pDevice{ nullptr };
	ComPtr<IDXGISwapChain> pGISwapChain{nullptr};
	ComPtr<ID3D11DeviceContext> pDeviceContext{ nullptr };
	ComPtr<ID3D11RenderTargetView> pRenderTargetView{nullptr};
	ComPtr<ID3DBlob> pBlobVS{ nullptr };
	ComPtr<ID3DBlob> pBlobPS{ nullptr };
	ComPtr<ID3D11VertexShader> pVertexShader{ nullptr };
	ComPtr<ID3D11PixelShader> pPixelShader{ nullptr };
	ComPtr<ID3D11Buffer> pBuffer{ nullptr };
	ComPtr<ID3D11InputLayout> pInputLayout{ nullptr };
	ComPtr<ID3D11Buffer> pIndexBuffer{ nullptr };
	ComPtr<ID3D11Buffer> pConstBuffer{ nullptr };
	ComPtr<ID3D11Texture2D> pTex2D{ nullptr };
	ComPtr<ID3D11ShaderResourceView> pShaderResView{ nullptr };
	ComPtr<ID3D11SamplerState> pSampler{ nullptr };
	ComPtr<ID3D11Resource> pResource{ nullptr };
	float LookAtX = 0;
	float LookAtY = 0;
}d3dx;

//ðåàëèçàöèÿ
void GraphicEngine::RenderClearBuffer(float red, float green, float blue)
{
	const float color[] = { red,green,blue,1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
}

void GraphicEngine::Present(bool vSync)
{
	if (vSync)
	{
		Logg.Log(pGISwapChain->Present(1u, 0u), "Present + vSync");
	}
	else
	{
		Logg.Log(pGISwapChain->Present(0u, 0u), "Present - vSync");
	}
}

void GraphicEngine::DrawObject(float x, float y, float z,float width, float height,float ZAngle,TypeObject typeOBJ,const wchar_t* filename)
{
	BuildListBuffer ListBuffer(x, y, z, width, height, ZAngle, LookAtX, LookAtY);
	CreateTextureBuffer(filename);
	CreateMatrixBuffer(ListBuffer.GetMatrix(typeOBJ));
	CreateVectorBuff(ListBuffer.GetVectorList(typeOBJ));
	UINT IndexCount = CreateIndexBuff(ListBuffer.GetIndex(typeOBJ));
	SetShadersVSPS(typeOBJ);
	CreateInputLayer(ListBuffer.GetElementDesc(typeOBJ));
	SetViewports(0, 1, 0, 0, 1u);
	RenderDrawIndex(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexCount);
}
