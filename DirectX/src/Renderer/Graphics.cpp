#include "pch.h"
#include "Graphics.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include "ImGui/imgui_impl_dx11.h"

#include "Core/Window.h"

// TEMP
#include "Renderer/Bindable/Buffer.h"
#include "Renderer/Bindable/Shader.h"
#include "Renderer/Bindable/Topology.h"
#include "Renderer/Bindable/InputLayout.h"

namespace dx = DirectX;

struct Vertex
{
	float x, y, z;
};

Graphics::Graphics(HWND hWnd, Window* wnd)
	: m_Wnd(wnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwap, &pDevice, nullptr, &pContext);

	ID3D11Resource* pBackBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget);
	pBackBuffer->Release();

	ID3D11DepthStencilState* pDSState;
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	pContext->OMSetDepthStencilState(pDSState, 1);

	ID3D11Texture2D* pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = { };
	descDepth.Width = m_Wnd->GetClientWidth();
	descDepth.Height = m_Wnd->GetClientHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = { };
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDSV);

	pContext->OMSetRenderTargets(1, &pTarget, pDSV);

	D3D11_VIEWPORT vp;
	vp.Width = m_Wnd->GetClientWidth();
	vp.Height = m_Wnd->GetClientHeight();
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);

	ImGui_ImplDX11_Init(pDevice, pContext);

	/*pDSState->Release();
	pDepthStencil->Release();*/
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();

	if (pSwap)
		pSwap->Release();
	if (pDevice)
		pDevice->Release();
	if (pContext)
		pContext->Release();
	if (pTarget)
		pTarget->Release();
	if (pDSV)
		pDSV->Release();
}

void Graphics::ClearRenderBuffer(float red, float green, float blue, float alpha /*= 1.0f*/)
{
	const float color[] = { red, green, blue, alpha };
	pContext->ClearRenderTargetView(pTarget, color);
	pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::Flush()
{
	pSwap->Present(1, 0);
}

void Graphics::DrawIndexed(size_t count)
{
	D3D11_VIEWPORT vp;
	vp.Width = m_Wnd->GetClientWidth();
	vp.Height = m_Wnd->GetClientHeight();
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);

	pContext->DrawIndexed(count, 0, 0);
}

dx::XMMATRIX Graphics::GetProjectionMatrix() const
{
	return dx::XMMatrixPerspectiveLH(1.0f, 9 / 16.0f, 0.5f, 2000.0f);
}
