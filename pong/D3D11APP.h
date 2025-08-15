#pragma once
#include "BaseStructures.h"
#include "MathShaders.h"
#include <wrl.h>

//ТУТОРИАЛЫ
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
	void Draw2DBox(float x, float y, float width, float height);


private:
//модули для создания различных способов рисования



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

void GraphicEngine::Draw2DBox(float x, float y, float width, float height)
{
	float xLeft = (window.width / 2 - x) / (window.width / 2);
	float xRight = (x + width - window.width / 2) / (window.width / 2);
	float yTop = (window.height / 2 - y) / (window.height / 2);
	float yBottom = (y + height - window.height / 2) / (window.height / 2);
	VEC2 VertexList[] =
	{
		{-xLeft,-yBottom},
		{-xLeft,yTop},
		{xRight,yTop},
		{xRight,yTop},
		{xRight,-yBottom},
		{-xLeft,-yBottom},
	};
	D3D11_BUFFER_DESC BD{};
	BD.ByteWidth = sizeof(VertexList);
	BD.StructureByteStride = sizeof(VEC2);
	BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BD.Usage = D3D11_USAGE_DEFAULT;
	BD.CPUAccessFlags = 0u;
	BD.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA SD{};
	SD.pSysMem = VertexList;
	Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");
	const UINT stride = sizeof(VEC2);
	const UINT offset = 0u;
	pDeviceContext->IASetVertexBuffers(0u, 1u, pBuffer.GetAddressOf(), &stride, &offset);
	Logg.Log(D3DReadFileToBlob(L"VertexShader.cso", &pBlobVS), "D3DReadFileToBlob");
	Logg.Log(D3DReadFileToBlob(L"PixelShader.cso", &pBlobPS), "D3DReadFileToBlob");
	Logg.Log(pDevice->CreateVertexShader(pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), nullptr, &pVertexShader), "CreateVertexShader");
	pDeviceContext->VSSetShader(pVertexShader.Get(), 0, 0);
	Logg.Log(pDevice->CreatePixelShader(pBlobPS->GetBufferPointer(), pBlobPS->GetBufferSize(), nullptr, &pPixelShader), "CreatePixelShader");
	pDeviceContext->PSSetShader(pPixelShader.Get(), 0, 0);
	D3D11_VIEWPORT VP{};
	VP.Width = window.width;
	VP.Height = window.height;
	VP.MinDepth = 0.0f;
	VP.MaxDepth = 1.0f;
	VP.TopLeftX = 0;
	VP.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &VP);
	const D3D11_INPUT_ELEMENT_DESC ide[] =
	{ {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} };
	Logg.Log(pDevice->CreateInputLayout(ide, (UINT)size(ide), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
	pDeviceContext->IASetInputLayout(pInputLayout.Get());
	pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->Draw((UINT)size(VertexList), 0u);
}








