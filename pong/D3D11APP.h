#pragma once
#include "BaseStructures.h"
#include <wrl.h>

//ТУТОРИАЛЫ
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

	void CreateVertexShader(const wchar_t* namefile)
	{
		Logg.Log(D3DCompileFromFile(namefile, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pBlobVS, &pBlobError), "D3DCompileVS");
		Logg.Log(pDevice->CreateVertexShader(pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), NULL, &pVertexShader), "CreateVertexShader");
		ElenetDesc[0].SemanticName = "POSITION";
		ElenetDesc[0].SemanticIndex = 0;
		ElenetDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ElenetDesc[0].InputSlot = 0;
		ElenetDesc[0].AlignedByteOffset = 0;
		ElenetDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ElenetDesc[0].InstanceDataStepRate = 0;
		pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
		pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	}
	void CreatePixelShader(const wchar_t* namefile)
	{
		Logg.Log(D3DCompileFromFile(namefile, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pBlobPS, &pBlobError), "D3DCompilePS");
		Logg.Log(pDevice->CreatePixelShader(pBlobPS->GetBufferPointer(), pBlobPS->GetBufferSize(), NULL, &pPixelShader), "CreatePixelShader");
		ElenetDesc[1].SemanticName = "COLOR";
		ElenetDesc[1].SemanticIndex = 0;
		ElenetDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		ElenetDesc[1].InputSlot = 0;
		ElenetDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		ElenetDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ElenetDesc[1].InstanceDataStepRate = 0;

		auto numElement = sizeof(ElenetDesc) / sizeof(ElenetDesc[0]);
		Logg.Log(pDevice->CreateInputLayout(ElenetDesc, numElement, pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");

			float VertexList[][3]
			 {
				{-0.5f,-0.5f,0.0f},
				{0.0f,0.5f,0.0f},
				{0.5f,-0.5f,0.0f}
			 };
			 const int SizeList = sizeof(VertexList)/sizeof(VertexList[0]);
			 
			 D3D11_BUFFER_DESC BufferDesc{};
			 BufferDesc.CPUAccessFlags = 0;
			 BufferDesc.MiscFlags = 0;
			 BufferDesc.StructureByteStride = 0;
			 BufferDesc.ByteWidth = sizeof(VertexList) * SizeList;
			 BufferDesc.Usage = D3D11_USAGE_DEFAULT;
			 

			 D3D11_SUBRESOURCE_DATA SubData{};
			 SubData.pSysMem = VertexList;

			 unsigned int stride = sizeof(VertexList);

			 Logg.Log(pDevice->CreateBuffer(&BufferDesc, &SubData, &pBuffer), "CreateBuffer");
			 pDeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &stride, 0);

	}

	void Present ()
	{
	Logg.Log(pGISwapChain->Present(1u, 0u), "Present");
	}

	void RenderClearBuffer(float red, float green,float blue)
	{
		const float color[] = { red,green,blue,1.0f };
		pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
		/*pDeviceContext->Draw(3u, 0u);*/
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
	ComPtr<ID3DBlob> pBlobError{};
	ComPtr<ID3D11VertexShader> pVertexShader{};
	ComPtr<ID3D11PixelShader> pPixelShader{};
	ComPtr<ID3D11Buffer> pBuffer{};
	ComPtr<ID3D11InputLayout> pInputLayout{};
	D3D11_INPUT_ELEMENT_DESC ElenetDesc[2];
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
	d3dx.CreateVertexShader(L"VertexShader.hlsl");
	d3dx.CreatePixelShader(L"PixelShader.hlsl");
}

void AppGame::Render()
{
	d3dx.Present();
	float sin_ = sin(Timer.TimePeak()) / 2.0f + 0.5f;
	d3dx.RenderClearBuffer(sin_, sin_, 1.0f);
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