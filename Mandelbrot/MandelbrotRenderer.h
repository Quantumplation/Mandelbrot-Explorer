#pragma once

#include <Windows.h>
#include <D3D11.h>
#include <D3Dx11.h>
#include <D3DX10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

extern ID3D11Device			*g_Device;
extern ID3D11DeviceContext	*g_DeviceContext;
extern IDXGISwapChain		*g_SwapChain;

class CMandelbrot
{
public:
	CMandelbrot(HWND hWnd);
	~CMandelbrot();

	void Draw();
private:
};