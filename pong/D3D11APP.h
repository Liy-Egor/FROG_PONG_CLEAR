#pragma once
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
	void Draw2DBox(float x, float y, float z, float width, float height, float colorDelta,float ZAngle);

private:
//модули для конкретных случаев
	
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
	
	void Set2DInputLayer()
	{
		const D3D11_INPUT_ELEMENT_DESC ide[] =
		{ 
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		Logg.Log(pDevice->CreateInputLayout(ide, (UINT)size(ide), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout), "CreateInputLayout");
		pDeviceContext->IASetInputLayout(pInputLayout.Get());
	}

	void CreateConstBuffer(float ZAngle)
	{
		float PropScreen = (float)window.height/(float)window.width;

		const ConstBuffer matrix =
		{
			{
				XMMatrixTranspose(
					/*XMMatrixScaling(1,PropScreen,1) **/
					XMMatrixTranslation(0,0,0) *
					XMMatrixPerspectiveLH(PropScreen,1,0.1f,30.0f) *
					/*XMMatrixRotationX(ZAngle) **/
					/*XMMatrixRotationY(ZAngle) **/
					XMMatrixRotationZ(ZAngle) 
				)
			}
		};

		D3D11_BUFFER_DESC BD{};
		BD.ByteWidth = sizeof(matrix.Translation.matrx);
		BD.StructureByteStride = 0u;
		BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BD.Usage = D3D11_USAGE_DYNAMIC;
		BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BD.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA SD{};
		SD.pSysMem = static_cast<const void*>(&matrix.Translation.matrx);
		Logg.Log(pDevice->CreateBuffer(&BD, &SD, &pConstBuffer), "CreateBuffer");
		pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
	}

private:

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
	void SetShadersVSPS(const wchar_t* VSNameFilecso, const wchar_t* PSNameFilecso)
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

//реализация
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

void GraphicEngine::Draw2DBox(float x, float y,float z, float width, float height, float colorDelta, float ZAngle)
{
	BuildListBuffer ListBuffer(x, y, z, width, height, colorDelta);
	CreateVectorBuff(ListBuffer.GetVectorList(TypeObject::Box2D));

	UINT IndexCount = Set2DIndexBuff<VEC3>(); //автоматизировать
	CreateConstBuffer(ZAngle); //автоматизировать

	SetShadersVSPS(L"2DVertexShader.cso", L"2DPixelShaderBlack.cso");
	SetViewports(0, 1, 0, 0, 1u);
	Set2DInputLayer(); //автоматизировать
	RenderDrawIndex(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexCount);
}








