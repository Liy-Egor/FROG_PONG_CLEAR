#pragma once
#include "MathShaders.h"
#include <wrl.h>

//ÒÓÒÎÐÈÀËÛ
//https://www.youtube.com/@PardCode
//https://www.rastertek.com/tutdx11win10.html
//https://learn.microsoft.com/ru-ru/windows/win32/api/d3d11/
//https://www.youtube.com/watch?v=YFrCrasocX8&list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&index=19

using namespace Microsoft::WRL;
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
	void DrawObject(
		float x, float y,float z, 
		float width, float height, 
		float colorDelta, 
		float XAngle, float YAngle, float ZAngle,
		const wchar_t* ShaderVS, const wchar_t* ShaderPS,
		float MinDepth, float MaxDepth, float TopLeftX, float TopLeftY, UINT countViewports,
		D3D_PRIMITIVE_TOPOLOGY MetodDraw,
		TypeObject typeObject
	);

private:
	UINT IndexCount = 0u;
	void SetBuffer(TypeBuffer TypeBuff, TypeObject TypeObj,
		float xLeft, float xRight,
		float yTop, float yBottom,
		float zBack, float zFront)
	{
		vector<VEC3>BUFF;
		if(TypeBuff == TypeBuffer::Vertex)
		{
			switch (TypeObj)
			{
			case TypeObject::Box2D:
			{
				BUFF.push_back({ -xLeft ,-yBottom ,zFront });
				BUFF.push_back({ -xLeft ,yTop ,zFront });
				BUFF.push_back({ xRight ,yTop ,zFront });
				BUFF.push_back({ xRight ,-yBottom ,zFront });
				break;
			}
			}
		}
		else if (TypeBuff == TypeBuffer::Index)
		{
			switch (TypeObj)
			{
			case TypeObject::Box2D:
			{
				BUFF.push_back({ 0 ,1 ,2 });
				BUFF.push_back({ 2 ,3 ,0 });
				IndexCount = BUFF.size();
				break;
			}

			}

		}
		else if (TypeBuff == TypeBuffer::Constant)
		{
			switch (TypeObj)
			{
			case TypeObject::Box2D:
			{
				float PropScreen = (float)window.height / (float)window.width;
				BUFF.push_back(
					{		XMMatrixTranspose(
							XMMatrixScaling(1,PropScreen,1) *
							XMMatrixRotationX(xLeft) *
							XMMatrixRotationY(xRight) *
							XMMatrixRotationZ(yTop)
							)
							});
				break;
			}
			}
		}


		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(BUFF);
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = &BUFF;
		switch (TypeBuff)
		{
		case TypeBuffer::Vertex:
		{
			BD.StructureByteStride = sizeof(BUFF);
			BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			BD.Usage = D3D11_USAGE_DEFAULT;
			BD.CPUAccessFlags = 0u;
			break;
		}
		case TypeBuffer::Constant:
		{
			BD.StructureByteStride = 0u;
			BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			BD.Usage = D3D11_USAGE_DYNAMIC;
			BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		}
		case TypeBuffer::Index:
		{
			BD.StructureByteStride = sizeof(BUFF);
			BD.BindFlags = D3D11_BIND_INDEX_BUFFER;
			BD.Usage = D3D11_USAGE_DEFAULT;
			BD.CPUAccessFlags = 0u;
			break;
		}

		}
		
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");
		if (TypeBuff == TypeBuffer::Vertex)
		{
			const UINT stride = sizeof(BUFF);
			const UINT offside = 0u;
			pDeviceContext->IASetVertexBuffers(0u, 1u, pBuffer.GetAddressOf(), &stride, &offside);
		}
		else if (TypeBuff == TypeBuffer::Constant)
		{
			pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
		}
		else if (TypeBuff == TypeBuffer::Index)
		{
			pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
		}
		BUFF.clear();
	}
	void SetShaders(const wchar_t* VSNameFilecso, const wchar_t* PSNameFilecso)
	{
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
	void SetInputLayer(TypeObject typeObject)
	{
		switch (typeObject)
		{
		case TypeObject::Box2D:
		{
			const D3D11_INPUT_ELEMENT_DESC Layer[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			Logg.Log(pDevice->CreateInputLayout(Layer, (UINT)size(Layer), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
		break;
		}
		case TypeObject::Triangle2D:
		{
			const D3D11_INPUT_ELEMENT_DESC Layer[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			Logg.Log(pDevice->CreateInputLayout(Layer, (UINT)size(Layer), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
			break;
		}
		case TypeObject::Box3D:
		{
			const D3D11_INPUT_ELEMENT_DESC Layer[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			Logg.Log(pDevice->CreateInputLayout(Layer, (UINT)size(Layer), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
			break;
		}

		}

		pDeviceContext->IASetInputLayout(pInputLayout.Get());
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
	ComPtr<IDXGISwapChain> pGISwapChain{nullptr};
	ComPtr<ID3D11Device> pDevice{ nullptr };
	ComPtr<ID3D11DeviceContext> pDeviceContext{ nullptr };
	ComPtr<ID3D11RenderTargetView> pRenderTargetView{nullptr};
	ComPtr<ID3D11Resource> pResource{ nullptr };
	ComPtr<ID3DBlob> pBlobVS{ nullptr };
	ComPtr<ID3DBlob> pBlobPS{ nullptr };
	ComPtr<ID3D11VertexShader> pVertexShader{ nullptr };
	ComPtr<ID3D11PixelShader> pPixelShader{ nullptr };
	ComPtr<ID3D11Buffer> pBuffer{ nullptr };
	ComPtr<ID3D11InputLayout> pInputLayout{ nullptr };
	ComPtr<ID3D11Buffer> pIndexBuffer{ nullptr };
	ComPtr<ID3D11Buffer> pConstBuffer{ nullptr };
}d3dx;

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

void GraphicEngine::DrawObject(
	float x, float y,float z, 
	float width, float height, 
	float colorDelta, 
	float XAngle, float YAngle, float ZAngle,
	const wchar_t* ShaderVS, const wchar_t* ShaderPS,
	float MinDepth, float MaxDepth, float TopLeftX, float TopLeftY, UINT countViewports,
	D3D_PRIMITIVE_TOPOLOGY MetodDraw,
	TypeObject typeObject)
{
	float xLeft = (window.width / 2 - x) / (window.width / 2);
	float xRight = ((x + width) - (window.width / 2)) / (window.width / 2);
	float yTop = (window.height / 2 - y) / (window.height / 2);
	float yBottom = (y + height - window.height / 2) / (window.height / 2);
	float zBack = 0;
	float zFront = 1;
	/*if(typeObject != TypeObject::Box2D || typeObject != TypeObject::Triangle2D)
	{
		zBack = (z + width - window.width / 2) / (window.width / 2);
		zFront = (window.width / 2 - z) / (window.width / 2);
	}*/

	SetBuffer(TypeBuffer::Vertex, typeObject, xLeft, xRight, yTop, yBottom, zBack, zFront);
	SetBuffer(TypeBuffer::Index, typeObject, 0, 0, 0, 0, 0, 0);
	SetBuffer(TypeBuffer::Constant, typeObject, XAngle, YAngle, ZAngle, 0, 0, 0);
	SetShaders(ShaderVS, ShaderPS);
	SetViewports(MinDepth, MaxDepth, TopLeftX, TopLeftY, countViewports);
	SetInputLayer(typeObject);
	RenderDrawIndex(MetodDraw, IndexCount);
}
