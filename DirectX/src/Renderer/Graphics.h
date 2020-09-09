#pragma once

#include "Core/Win.h"

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

class Window;

class Graphics
{
	friend class Bindable;
public:
	Graphics(HWND hWnd, Window* window);
	~Graphics();

	void ClearRenderBuffer(float red, float green, float blue, float alpha = 1.0f);
	void Flush();

	void DrawIndexed(size_t count);

	DirectX::XMMATRIX GetProjectionMatrix() const;
private:
	IDXGISwapChain* pSwap;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	ID3D11RenderTargetView* pTarget;
	ID3D11DepthStencilView* pDSV;

	Window* m_Wnd;
};
