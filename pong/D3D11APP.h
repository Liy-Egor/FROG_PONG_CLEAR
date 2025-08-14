#pragma once
#include "BaseStructures.h"
#include <wrl.h>
#include "MathShaders.h"

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

	void Draw()
	{
		//создание вершин
		const VEC2 VertexList[] =
		{
				{-0.5f,-0.5f},
				{-0.5f,0.5f},
				{0.5f,0.5f},
				{0.5f,0.5f},
				{0.5f,-0.5f},
				{-0.5f,-0.5f},
		};
		//дискриптор вершин
		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(VertexList);
		BD.StructureByteStride = sizeof(VEC2);
		BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BD.Usage = D3D11_USAGE_DEFAULT;
		BD.CPUAccessFlags = 0u;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = VertexList;

		//создание буфера
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");

		//привязка вершин к буферу
		const UINT stride = sizeof(VEC2);
		const UINT offset = 0u;
		pDeviceContext->IASetVertexBuffers(0u, 1u, pBuffer.GetAddressOf(), &stride, &offset);
		Logg.Log(D3DReadFileToBlob(L"VertexShader.cso", &pBlobVS), "D3DReadFileToBlob");
		Logg.Log(D3DReadFileToBlob(L"PixelShader.cso", &pBlobPS), "D3DReadFileToBlob");
		//шейдеры
		Logg.Log(pDevice->CreateVertexShader(pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), nullptr, &pVertexShader), "CreateVertexShader");
		pDeviceContext->VSSetShader(pVertexShader.Get(), 0, 0);

		Logg.Log(pDevice->CreatePixelShader(pBlobPS->GetBufferPointer(), pBlobPS->GetBufferSize(), nullptr, &pPixelShader), "CreatePixelShader");
		pDeviceContext->PSSetShader(pPixelShader.Get(), 0, 0);

		D3D11_VIEWPORT VP{};
		VP.Width = 1280;
		VP.Height = 720;
		VP.MinDepth = 0.0f;
		VP.MaxDepth = 1.0f;
		VP.TopLeftX = 0;
		VP.TopLeftY = 0;
		pDeviceContext->RSSetViewports(1, &VP);

		//входная разметка
		const D3D11_INPUT_ELEMENT_DESC ide[] =
		{ {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}};

		Logg.Log(pDevice->CreateInputLayout(ide, (UINT)size(ide), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
		pDeviceContext->IASetInputLayout(pInputLayout.Get());

		pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//отрисовка триугольника
		pDeviceContext->Draw((UINT)size(VertexList), 0u);
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
	ComPtr<ID3DBlob> pBlob{};
	ComPtr<ID3DBlob> pBlobVS{};
	ComPtr<ID3DBlob> pBlobPS{};
	ComPtr<ID3D11VertexShader> pVertexShader{};
	ComPtr<ID3D11PixelShader> pPixelShader{};
	ComPtr<ID3D11Buffer> pBuffer{};
	ComPtr<ID3D11InputLayout> pInputLayout{};
}d3dx;

class AppGame
{
public:
	AppGame(){};
	
	void FrameGo()
	{
		MSG msg;
		BOOL gbool = true;
		Init();
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
	void Init();
}App;

void AppGame::Init()
{
	/*d3dx.CreateVertexShader(L"VertexShader.hlsl");
	d3dx.CreatePixelShader(L"PixelShader.hlsl");*/
	//d3dx.DrawTriangle();
}

void AppGame::Render()
{
	//	float sin_ = sin(Timer.TimePeak()) / 2.0f + 0.5f;
	d3dx.RenderClearBuffer(0.0f, 1.0f, 1.0f);
	d3dx.Draw();
	d3dx.Present();
}

void AppGame::UpdateApp(MSG msg)
{
	




}

	////установка шейдеров
 //	float VertexList[][3]
	//{
	//	{-0.5f,-0.5f,0.0f},
	//	{0.0f,0.5f,0.0f},
	//	{0.5f,-0.5f,0.0f}
	//};
	//const UINT SizeList = sizeof(VertexList);

	//D3D11_BUFFER_DESC BufferDesc{};
	//BufferDesc.ByteWidth = sizeof(BufferDesc);
	//BufferDesc.Usage = D3D11_USAGE_DEFAULT;

	//D3D11_SUBRESOURCE_DATA SubData{};
	//SubData.pSysMem = VertexList;

	//Logg.Log(pDevice->CreateBuffer(&BufferDesc, &SubData, &pBuffer), "CreateBuffer");
	//pDeviceContext->IASetVertexBuffers(0u, 3u, &pBuffer, &SizeList, 0u);

	////входная разметка для шейдера
	//D3D11_INPUT_ELEMENT_DESC ElementDesc[] = {
	//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },};
	//int sizeElementDesc = _countof(ElementDesc);
	//auto VertexShaderLength = pBlob->GetBufferSize();
	//Logg.Log(pDevice->CreateInputLayout(ElementDesc, sizeElementDesc, pBlob->GetBufferPointer(), VertexShaderLength, pInputLayout.ReleaseAndGetAddressOf()), "CreateInputLayout");//ошибка

	////добавление слоя
	//pDeviceContext->IASetInputLayout(pInputLayout.Get());

	//D3D11_VIEWPORT ViewPort{};
	//ViewPort.Width = 1280;
	//ViewPort.Height = 720;
	//ViewPort.MinDepth = 0.0f;
	//ViewPort.MaxDepth = 1.0f;

	//pDeviceContext->RSSetViewports(1, &ViewPort);

	//pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);