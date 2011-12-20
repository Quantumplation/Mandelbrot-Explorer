#include "MandelbrotRenderer.h"

ID3D11Device			*g_Device;
ID3D11DeviceContext		*g_DeviceContext;
IDXGISwapChain			*g_SwapChain;
ID3D11RenderTargetView *g_BackBuffer;

CMandelbrot::CMandelbrot(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC swapDesc;

	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = hWnd;
    swapDesc.SampleDesc.Count = 4;
    swapDesc.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &swapDesc,
                                  &g_SwapChain,
                                  &g_Device,
                                  NULL,
                                  &g_DeviceContext);

	ID3D11Texture2D *pBackBuffer;
	g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_BackBuffer);
	pBackBuffer->Release();

	g_DeviceContext->OMSetRenderTargets(1, &g_BackBuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= 800;
	viewport.Height		= 600;

	g_DeviceContext->RSSetViewports(1, &viewport);
}

CMandelbrot::~CMandelbrot()
{
	g_SwapChain->Release();
	g_BackBuffer->Release();
	g_Device->Release();
	g_DeviceContext->Release();
}

void CMandelbrot::Draw()
{
	g_DeviceContext->ClearRenderTargetView(g_BackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	g_SwapChain->Present(0, 0);
}