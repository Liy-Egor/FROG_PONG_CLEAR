#pragma once
#include "BaseStructures.h"
#include <wrl.h>

//ÒÓÒÎÐÈÀËÛ
//https://www.youtube.com/@PardCode
//https://www.rastertek.com/tutdx11win10.html
//https://learn.microsoft.com/ru-ru/windows/win32/api/d3d11/

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
		(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&SWdesc,
			&pGISwapChain,
			&pDevice,
			nullptr,
			&pDeviceContext
		), "CreateDevice");

		Logg.Log(pGISwapChain->GetBuffer(0, IID_PPV_ARGS(&pResource)), "GetBuffer");
		Logg.Log(pDevice->CreateRenderTargetView(pResource.Get(), nullptr, &pRenderTargetView), "CreateRenderTargetView");
	}

	void Present ()
	{
	Logg.Log(pGISwapChain->Present(1u, 0u), "Present");
	}

	void RenderClearBuffer(float red, float green,float blue)
	{
		const float color[] = { red,green,blue,1.0f };
		pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	}
	
	~GraphicEngine()
	{


	}

	GraphicEngine(const GraphicEngine&) = delete;
	GraphicEngine(GraphicEngine&) = delete;
	GraphicEngine& operator = (const GraphicEngine) = delete;
	GraphicEngine& operator = (GraphicEngine&) = delete;
private:
	HRESULT hr;

	ComPtr<IDXGISwapChain> pGISwapChain{nullptr};
	ComPtr<ID3D11Device> pDevice{};
	ComPtr<ID3D11DeviceContext> pDeviceContext{};
	ComPtr<ID3D11RenderTargetView> pRenderTargetView{nullptr};
	ComPtr<ID3D11Resource> pResource{};

}d3dx;

class AppGame
{
public:
	AppGame(){};
	
	void FrameGo()
	{
		MSG msg;
		BOOL gbool = true;
		while (gbool)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					gbool = false;
					break;
				}
				UpdateApp(msg);

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Render();
		}
	}
private:
	void UpdateApp(MSG msg);
	void Render();
}App;

void AppGame::Render()
{
	d3dx.Present();
	d3dx.RenderClearBuffer(0.8f, 1.0f, 0.6f);
}

void AppGame::UpdateApp(MSG msg)
{
	




}