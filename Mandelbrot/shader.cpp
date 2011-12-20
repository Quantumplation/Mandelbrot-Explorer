#include "shader.h"

Shader::Shader(ID3D11Device& device, LPCTSTR file_name, 
			   LPCSTR vertex_shader_name, LPCSTR pixel_shader_name) 
			   : vertex_shader_(NULL), pixel_shader_(NULL)
{
	if(vertex_shader_name)
	{
		ID3D10Blob *vertex_shader_blob;
		ID3D10Blob *error_blob;
		D3DX11CompileFromFile(file_name, 0, 0, vertex_shader_name, "vs_5_0", 0, 
							  0, 0, &vertex_shader_blob, &error_blob, NULL);
		if(error_blob)
		{
			
		}
		device.CreateVertexShader(vertex_shader_blob->GetBufferPointer(), 
								   vertex_shader_blob->GetBufferSize(), NULL,
								   &vertex_shader_);
	}

	if(pixel_shader_name)
	{
		ID3D10Blob *pixel_shader_blob;

		D3DX11CompileFromFile(file_name, 0, 0, pixel_shader_name, "ps_5_0", 0, 0, 0, 
							  &pixel_shader_blob, 0, 0);
		device.CreatePixelShader(pixel_shader_blob->GetBufferPointer(),
								  pixel_shader_blob->GetBufferSize(), NULL,
								  &pixel_shader_);
	}
}

Shader::~Shader()
{
	if(vertex_shader_) vertex_shader_->Release();
	if(pixel_shader_) pixel_shader_->Release();
}

void Shader::Activate(ID3D11DeviceContext& device_context)
{
	if(vertex_shader_) device_context.VSSetShader(vertex_shader_, 0, 0);
	if(pixel_shader_) device_context.PSSetShader(pixel_shader_, 0, 0);
}