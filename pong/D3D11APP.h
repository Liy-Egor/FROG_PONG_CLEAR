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
	void Draw2DBox(float x, float y, float width, float height, float colorDelta, float XAngle, float YAngle, float ZAngle);
	void Draw2DLine(float x, float y, float width, float height, float colorDelta, float XAngle, float YAngle, float ZAngle);
	void Draw3DBox(float x, float y, float z, float width, float height);
	void Draw3DLine(float x, float y, float z, float width, float height);


private:
//модули для конкретных случаев
	template <typename T>
	UINT Create2DBuff(float x, float y, float width, float height,float colorDelta)
	{
		float xLeft = (window.width / 2 - x) / (window.width / 2);
		float xRight = (x + width - window.width / 2) / (window.width / 2);
		float yTop = (window.height / 2 - y) / (window.height / 2);
		float yBottom = (y + height - window.height / 2) / (window.height / 2);

		T VertexList[] =
		{
			{-xLeft,-yBottom,  1 * colorDelta,0.3,    1 / colorDelta},
			{-xLeft,yTop,      1 / colorDelta,      1 * colorDelta,0.3},
			{xRight,yTop,      1 / colorDelta,0.3,    1 / colorDelta},
			{xRight,-yBottom,0.3,1 / colorDelta,      1 * colorDelta},
		};
		
		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(VertexList);
		BD.StructureByteStride = sizeof(T);
		BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BD.Usage = D3D11_USAGE_DEFAULT;
		BD.CPUAccessFlags = 0u;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = VertexList;
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");
		return (UINT)size(VertexList);
	}
	void Set2DInputLayer()
	{
		const D3D11_INPUT_ELEMENT_DESC ide[] =
		{ 
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		Logg.Log(pDevice->CreateInputLayout(ide, (UINT)size(ide), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
		pDeviceContext->IASetInputLayout(pInputLayout.Get());
	}

	template <typename T>
	UINT Set2DIndexBuff()
	{
		const unsigned short Indices[] =
		{
			0,1,2,
			2,3,0,
		};
		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(Indices);
		BD.StructureByteStride = sizeof(T);
		BD.BindFlags = D3D11_BIND_INDEX_BUFFER;
		BD.Usage = D3D11_USAGE_DEFAULT;
		BD.CPUAccessFlags = 0u;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = Indices;
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pIndexBuffer), "CreateBuffer");
		pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(),DXGI_FORMAT_R16_UINT,0u);
		return (UINT)size(Indices);
	}
	




	template <typename T>
	UINT Create3DBuff(float x, float y, float z, float width, float height)
	{
		float xLeft = (window.width / 2 - x) / (window.width / 2);
		float xRight = ((x + width) - (window.width / 2)) / (window.width / 2);
		float yTop = (window.height / 2 - y) / (window.height / 2);
		float yBottom = (y + height - window.height / 2) / (window.height / 2);
		float zBack = (z + width - window.width / 2) / (window.width / 2);
		float zFront = (window.width / 2 - z) / (window.width / 2);
		float fas = 0.05f;

		T VertexList[] =
		{
			//фронт
		/*	{-xLeft,-yBottom,zFront},
			{-xLeft,yTop,zFront},
			{xRight,yTop,zFront},
			{xRight,yTop,zFront},
			{xRight,-yBottom,zFront},
			{-xLeft,-yBottom,zFront},*/

			//left
			{-xLeft+fas,-yBottom-fas,zFront},
			{-xLeft+fas,-yBottom-fas,-zBack},
			{-xLeft+fas,yTop    -fas,-zBack},
			{-xLeft+fas,yTop    -fas,-zBack},
			{-xLeft+fas,yTop    -fas,zFront},
			{-xLeft+fas,-yBottom-fas,zFront},

			//////бэк
			//{-xLeft,-yBottom,-zBack},
			//{-xLeft,yTop,-zBack},
			//{xRight,yTop,-zBack},
			//{xRight,yTop,-zBack},
			//{xRight,-yBottom,-zBack},
			//{-xLeft,-yBottom,-zBack},

			////right
			//{xLeft,-yBottom,0.5f},
			//{xLeft,yTop,0.5f},
			//{xRight,yTop,0.5f},
			//{xRight,yTop,0.5f},
			//{xRight,-yBottom,0.5f},
			//{xLeft,-yBottom,0.5f},

			////вверх
			//{-xLeft,yBottom,-0.5f},
			//{-xLeft,yTop,-0.5f},
			//{xRight,yTop,-0.5f},
			//{xRight,yTop,-0.5f},
			//{xRight,yBottom,-0.5f},
			//{-xLeft,yBottom,-0.5f},

			////низ
			//{-xLeft,-yBottom,-0.5f},
			//{-xLeft,-yTop,-0.5f},
			//{xRight,-yTop,-0.5f},
			//{xRight,-yTop,-0.5f},
			//{xRight,-yBottom,-0.5f},
			//{-xLeft,-yBottom,-0.5f},
		};

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(VertexList);
		BD.StructureByteStride = sizeof(T);
		BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BD.Usage = D3D11_USAGE_DEFAULT;
		BD.CPUAccessFlags = 0u;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = VertexList;
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pBuffer), "CreateBuffer");
		return (UINT)size(VertexList);
	}
	void Set3DInputLayer()
	{
		const D3D11_INPUT_ELEMENT_DESC ide[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}

		};
		Logg.Log(pDevice->CreateInputLayout(ide, (UINT)size(ide), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
		pDeviceContext->IASetInputLayout(pInputLayout.Get());
	}


private:
//универсальные модули
	template <typename T>
	void SetShadersVSPS(const wchar_t* VSNameFilecso, const wchar_t* PSNameFilecso)
	{
		const UINT stride = sizeof(T);
		const UINT offset = 0u;
		pDeviceContext->IASetVertexBuffers(0u, 1u, pBuffer.GetAddressOf(), &stride, &offset);
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
	void RenderDraw(D3D_PRIMITIVE_TOPOLOGY MetodDraw,UINT ListCount)
	{
		pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);
		pDeviceContext->IASetPrimitiveTopology(MetodDraw);
		pDeviceContext->Draw(ListCount, 0u);
	}
	void RenderDrawIndex(D3D_PRIMITIVE_TOPOLOGY MetodDraw, UINT IndexCount)
	{
		pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);
		pDeviceContext->IASetPrimitiveTopology(MetodDraw);
		pDeviceContext->DrawIndexed(IndexCount, 0u, 0u);
	}

	void ModuleConstBuffer(ConstBuffer arr)
	{
		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(arr.Translation.matrx);
		BD.StructureByteStride = 0u;
		BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BD.Usage = D3D11_USAGE_DYNAMIC;
		BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = arr.Translation.matrx;
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pConstBuffer), "CreateBuffer");
		pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
	}
	void CreateConstBuffer(float XAngle, float YAngle, float ZAngle)
	{
		float PropScreen = (3.0f / 4.0f);

		const ConstBuffer Defult =
		{
			{
				1.0f,				0.0f,				0.0f,				0.0f,
				0.0f,				1.0f,				0.0f,				0.0f,
				0.0f,				0.0f,				1.0f,				0.0f,
				0.0f,				0.0f,				0.0f,				1.0f,
			}
		};
		const ConstBuffer xRotate =
		{
			{
				1.0f,				0.0f,								0.0f,				0.0f,
				0.0f,				PropScreen* cos(XAngle),			sin(XAngle),		0.0f,
				0.0f,				PropScreen * -sin(XAngle),			cos(XAngle),		0.0f,
				0.0f,				0.0f,								0.0f,				1.0f,
			}
		};
		const ConstBuffer yRotate =
		{
			{
				PropScreen* cos(YAngle),			0.0f,				PropScreen * -sin(YAngle),		0.0f,
				0.0f,								1.0f,				0.0f,				0.0f,
				sin(YAngle),						0.0f,				cos(YAngle),		0.0f,
				0.0f,								0.0f,				0.0f,				1.0f,
			}
		};
		const ConstBuffer zRotate =
		{
			{
				PropScreen* cos(ZAngle),			sin(ZAngle),		0.0f,				0.0f,
				PropScreen * -sin(ZAngle),			cos(ZAngle),		0.0f,				0.0f,
				0.0f,								0.0f,				1.0f,				0.0f,
				0.0f,								0.0f,				0.0f,				1.0f,
			}
		};

		/*ModuleConstBuffer(xRotate);
		ModuleConstBuffer(yRotate);*/
		ModuleConstBuffer(zRotate);
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

void GraphicEngine::Draw2DBox(float x, float y, float width, float height, float colorDelta, float XAngle, float YAngle, float ZAngle)
{
	UINT ListCount = Create2DBuff<VEC2>(x, y, width, height, colorDelta);
	SetShadersVSPS<VEC2>(L"2DVertexShader.cso", L"2DPixelShaderBlack.cso");
	SetViewports(0, 1, 0, 0, 1u);
	Set2DInputLayer();
	UINT IndexCount = Set2DIndexBuff<VEC2>();
	CreateConstBuffer(XAngle, YAngle, ZAngle);
	RenderDrawIndex(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexCount);
}

void GraphicEngine::Draw2DLine(float x, float y, float width, float height, float colorDelta,float XAngle, float YAngle, float ZAngle)
{
	UINT ListCount = Create2DBuff<VEC2>(x, y, width, height, colorDelta);
	SetShadersVSPS<VEC2>(L"2DVertexShader.cso", L"2DPixelShaderWhite.cso");
	SetViewports(0, 1, 0, 0, 1u);
	Set2DInputLayer();
	UINT IndexCount = Set2DIndexBuff<VEC2>();
	CreateConstBuffer(XAngle, YAngle, ZAngle);
	RenderDrawIndex(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, IndexCount);
}

void GraphicEngine::Draw3DBox(float x, float y, float z, float width, float height)
{
	UINT ListCount = Create3DBuff<VEC3>(x, y, z, width, height);
	SetShadersVSPS<VEC3>(L"3DVertexShader.cso", L"3DPixelShaderBlack.cso");
	SetViewports(0, 1, 0, 0, 1u);
	Set3DInputLayer();
	RenderDraw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, ListCount);
}

void GraphicEngine::Draw3DLine(float x, float y, float z, float width, float height)
{
	UINT ListCount = Create3DBuff<VEC3>(x, y, z, width, height);
	SetShadersVSPS<VEC3>(L"3DVertexShader.cso", L"3DPixelShaderWhite.cso");
	SetViewports(0, 1, 0, 0, 1u);
	Set3DInputLayer();
	RenderDraw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, ListCount);
}





