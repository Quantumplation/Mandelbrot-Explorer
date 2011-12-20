#include <string>
#include "mandelbrot.h"
#include "shader.h"
#include "sprite.h"

using namespace std;

Mandelbrot::Mandelbrot(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC swap_description;

	ZeroMemory(&swap_description, sizeof(DXGI_SWAP_CHAIN_DESC));

	swap_description.BufferCount = 1;
	swap_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_description.OutputWindow = hWnd;
    swap_description.SampleDesc.Count = 4;
    swap_description.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &swap_description,
                                  &swap_chain_,
                                  &device_,
                                  NULL,
                                  &device_context_);

	ID3D11Texture2D *back_buffer_texture;
	swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_texture);

	device_->CreateRenderTargetView(back_buffer_texture, NULL, &back_buffer_);
	back_buffer_texture->Release();

	device_context_->OMSetRenderTargets(1, &back_buffer_, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= 800;
	viewport.Height		= 600;

	device_context_->RSSetViewports(1, &viewport);
}

Mandelbrot::~Mandelbrot()
{
	for(map<string, Shader*>::const_iterator it = shaders_.begin(); 
		it != shaders_.end(); it++)
	{
		delete it->second;
	}

	swap_chain_->Release();
	back_buffer_->Release();
	device_->Release();
	device_context_->Release();
}

void Mandelbrot::CreateShader(const string& shader_name, LPCTSTR file_name, 
								LPCSTR vertex_shader, LPCSTR pixel_shader, 
								bool activate)
{
	shaders_[shader_name] = new Shader(*device_, file_name, vertex_shader, 
									   pixel_shader);
	if(activate) SetCurrentShader(shader_name);
}

void Mandelbrot::SetCurrentShader(const string& shader_name)
{
	if(shader_name != current_shader_)
	{
		current_shader_ = shader_name;
		shaders_[shader_name]->Activate(*device_context_);
	}
}

void Mandelbrot::CreateSprite()
{
	sprites_.push_back(new Sprite(*device_, *device_context_));
}

void Mandelbrot::Draw()
{
	device_context_->ClearRenderTargetView(back_buffer_, D3DXCOLOR(0,0.2,0.4,1));

	for(vector<Sprite*>::const_iterator it = sprites_.begin(); 
		it != sprites_.end(); it++)
	{
		(*it)->Activate(*device_context_);
		(*it)->Draw(*device_context_);
	}

	swap_chain_->Present(0, 0);
}