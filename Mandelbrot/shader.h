#ifndef SHADER_H
#define SHADER_H

#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

class Shader
{
public:
	Shader(ID3D11Device& device, LPCTSTR file_name, LPCSTR vertex_shader_name, 
		   LPCSTR pixel_shader_name);
	~Shader();

	void Activate(ID3D11DeviceContext& device_context);
private:
	ID3D11VertexShader *vertex_shader_;
	ID3D11PixelShader  *pixel_shader_;
	ID3D11InputLayout *input_layout_;
};

#endif