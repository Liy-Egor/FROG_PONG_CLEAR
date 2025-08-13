#pragma once
#include "BaseStructures.h"
#include <wrl.h>

using namespace Microsoft::WRL;

class AppGame
{
public:
	AppGame() :wnd(600, 600, "GameFrog") {};

	void FrameGo()
	{
		InitApp();

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

				UpdateApp();

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Render();
		}
	}
	HWND GetHwnd()
	{
		return wnd.GetHWND();
	}
private:
	void InitApp();
	Window wnd;
	void UpdateApp();
	void Render();
}App;

class GraphicEngine
{
public:
	GraphicEngine()
	{
		Logg.Log(hr = D3D11CreateDevice
		(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&pDevice,
			&FL,
			&pContext
		), "CreateDevice");

		SWdesc.BufferDesc.Width = 0u;
		SWdesc.BufferDesc.Height = 0u;
		SWdesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		SWdesc.BufferDesc.RefreshRate.Numerator = 0u;
		SWdesc.BufferDesc.RefreshRate.Denominator = 0u;
		SWdesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SWdesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SWdesc.SampleDesc.Count = 1u;
		SWdesc.SampleDesc.Quality = 0u;
		SWdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SWdesc.BufferCount = 2u;
		SWdesc.OutputWindow = App.GetHwnd();
		SWdesc.Windowed = TRUE;
		SWdesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SWdesc.Flags = 0u;

		Logg.Log(pDevice->QueryInterface(IID_PPV_ARGS(&pGIDevice)),"QueryInterface");
		Logg.Log(pGIDevice->GetParent(IID_PPV_ARGS(&pGIAdapter)), "pGIDevice->GetParent");
		Logg.Log(pGIAdapter->GetParent(IID_PPV_ARGS(&pGIFactory)), "pGIAdapter->GetParent");
		Logg.Log(pGIFactory->CreateSwapChain(*&pDevice, &SWdesc, &pGISwapChain), "CreateSwapChain");
		pDevice->CreateDeferredContext(0, &pDeviceContext);
		Logg.Log(pGISwapChain->GetBuffer(2u, IID_PPV_ARGS(&pTex2d)), "GetBuffer");

		TargetDC.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		TargetDC.ViewDimension = D3D11_RTV_DIMENSION_BUFFER;		

		Logg.Log(pDevice->CreateRenderTargetView(pResource.Get(), &TargetDC, &pRenderTargetView), "CreateRenderTargetView");
	}

	void ClearBuffer(float red, float green,float blue)
	{
	const float color[] = { red,green,blue,1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	}
	

	~GraphicEngine()
	{


	}
	//правило пяти
	GraphicEngine(const GraphicEngine&) = delete;
	GraphicEngine(GraphicEngine&) = delete;
	GraphicEngine& operator = (const GraphicEngine) = delete;
	GraphicEngine& operator = (GraphicEngine&) = delete;
private:
	D3D_FEATURE_LEVEL FL{
	D3D_FEATURE_LEVEL_11_0
	};
	HRESULT hr;
	ComPtr<ID3D11Device> pDevice{};
	ComPtr<ID3D11DeviceContext> pContext{};
	ComPtr<IDXGIDevice> pGIDevice{};
	ComPtr<IDXGIAdapter> pGIAdapter{};
	ComPtr<IDXGIFactory> pGIFactory{};
	ComPtr<IDXGISwapChain> pGISwapChain{0};
	DXGI_SWAP_CHAIN_DESC SWdesc{};
	ComPtr<ID3D11DeviceContext> pDeviceContext{};
	ComPtr<ID3D11RenderTargetView> pRenderTargetView{};
	ComPtr<ID3D11Texture2D> pTex2d{};
	ComPtr<ID3D11Resource> pResource{};
	D3D11_RENDER_TARGET_VIEW_DESC TargetDC;
};

void AppGame::InitApp()
{
	GraphicEngine d3dx;
}

void AppGame::Render()
{
	Sleep(1);
}

void AppGame::UpdateApp()
{
	
	
}