#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <Windows.h>
#include <map>
#include <vector>
#include <string>
#include <D3D11.h>
#include <D3Dx11.h>
#include <D3DX10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class Shader;
class Sprite;

class Mandelbrot
{
public:
	Mandelbrot(HWND hWnd);
	~Mandelbrot();

	void CreateShader(const std::string& shader_name, 
						LPCTSTR file_name, LPCSTR vertex_shader, 
						LPCSTR pixel_shader, LPCSTR texture, 
						bool activate = false);
	void SetCurrentShader(const std::string& shader_name);
	
	template<typename T>
	void SetShaderParameters(const std::string& shader_name, const T& data)
	{
		if(shaders_.find(shader_name) != shaders_.end())
		{
			shaders_[shader_name]->SetParameters<T>(data, *device_context_);
			shaders_[shader_name]->Activate(*device_context_);
		}
	}


	void CreateSprite();
	void Draw();

private:
	ID3D11Device			*device_;
	ID3D11DeviceContext		*device_context_;
	IDXGISwapChain			*swap_chain_;
	ID3D11RenderTargetView	*back_buffer_;

	ID3D11InputLayout		*layout_;

	std::vector<Sprite*> sprites_;
	std::map<std::string, Shader*> shaders_;
	std::string current_shader_;
};

#endif