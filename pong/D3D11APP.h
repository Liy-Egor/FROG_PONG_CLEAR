#pragma once
#include "MathShaders.h"
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
	void DrawObject(float x, float y, float z, float width, float height, float colorDelta,float ZAngle, TypeObject typeOBJ, float xPlayer, float yPlayer);

private:

	///////!!!!!!
	void CreateTextureBuffer()
	{
		D3D11_TEXTURE2D_DESC TXDC{};
		TXDC.Width = DATASAVE2[0].GetImages()->width;
		TXDC.Height = DATASAVE2[0].GetImages()->height;
		TXDC.MipLevels = 0;
		TXDC.ArraySize = 1;
		TXDC.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		TXDC.SampleDesc.Count = 1;
		TXDC.SampleDesc.Quality = 0;
		TXDC.Usage = D3D11_USAGE_DEFAULT;
		TXDC.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		TXDC.CPUAccessFlags = 0;
		TXDC.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		Logg.Log(pDevice->CreateTexture2D(&TXDC, NULL, &pTex2D), "CreateTexture2D");

		pDeviceContext->UpdateSubresource(pTex2D.Get(), 0,NULL, DATASAVE2[0].GetImages()->pixels, DATASAVE2[0].GetImages()->rowPitch, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVD{};
		SRVD.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		SRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVD.Texture2D.MostDetailedMip = 0;
		SRVD.Texture2D.MipLevels = 1;
		pDevice->CreateShaderResourceView(pTex2D.Get(), &SRVD, &pShaderResView); //ÈÇÌÅÍÈË
		pDeviceContext->PSSetShaderResources(0u, 1u, pShaderResView.GetAddressOf());

		D3D11_SAMPLER_DESC SDC{};
		SDC.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		SDC.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SDC.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SDC.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		pDevice->CreateSamplerState(&SDC, &pSampler);
		pDeviceContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}
	///////!!!!!!


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
		return Index.size();
		delete[](arr);
	}

	void CreateMatrixBuffer(vector<XMMATRIX> matrix)
	{
		XMMATRIX* arr = new XMMATRIX[matrix.size()];
		for (int i = 0; i < matrix.size(); ++i) {
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

void GraphicEngine::DrawObject(
	float x, float y, float z,
	float width, float height,
	float colorDelta,
	float ZAngle,
	TypeObject typeOBJ,
	float xPlayer, float yPlayer
)
{
	BuildListBuffer ListBuffer(x, y, z, width, height, colorDelta, ZAngle, xPlayer, yPlayer);

	UINT IndexCount = CreateIndexBuff(ListBuffer.GetIndex(typeOBJ));
	CreateVectorBuff(ListBuffer.GetVectorList(typeOBJ));

	
	CreateTextureBuffer();
	/*ListBuffer.GetImages(L"xxx.png")*/

	CreateMatrixBuffer(ListBuffer.GetMatrix(typeOBJ));
	if(typeOBJ == TypeObject::Box2D)
	{SetShadersVSPS(L"2DVertexShader.cso", L"2DPixelShaderBlack.cso");}
	if (typeOBJ == TypeObject::Box2DTEX)
	{
		SetShadersVSPS(L"TexVertexShader.cso", L"TexPixelShader.cso");
	}

	CreateInputLayer(ListBuffer.GetElementDesc(typeOBJ));

	SetViewports(0, 1, 0, 0, 1u);
	RenderDrawIndex(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexCount);
}



//for (int i = 0; i < DataImage.size(); ++i) {
//	arr[i] = DataImage[i];
//}

//unsigned int rowPitch = (WidthImage * 4) * sizeof(unsigned char);
//D3D11_TEXTURE2D_DESC TXDC{};
//TXDC.Width = WidthImage; //ØÈÐÈÍÀ ÈÇÎÁÐÀÆÄÅÍÈß
//TXDC.Height = HeightImage; //ÂÛÑÎÒÀ ÈÇÎÁÐÀÆÄÅÍÈß
//TXDC.MipLevels = 0;
//TXDC.ArraySize = 1;
//TXDC.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//TXDC.SampleDesc.Count = 1;
//TXDC.SampleDesc.Quality = 0;
//TXDC.Usage = D3D11_USAGE_DEFAULT;
//TXDC.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//TXDC.CPUAccessFlags = 0;
//TXDC.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
//Logg.Log(pDevice->CreateTexture2D(&TXDC, NULL, &pTex2D), "CreateTexture2D");

//pDeviceContext->UpdateSubresource(pTex2D.Get(), 0,NULL, static_cast<const void*>(&arr) , rowPitch, 0);


//D3D11_SUBRESOURCE_DATA SD{};
//SD.pSysMem = 0; //ËÈÑÒ ÈÇÎÁÐÀÆÅÍÈß
//SD.SysMemPitch = 0; //ØÈÐÈÍÀ ÍÀ ÑÀÉÇ
//pDevice->CreateTexture2D(&TXDC, &SD, &pTex2D);




