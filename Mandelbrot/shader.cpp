#include "shader.h"

Shader::Shader(ID3D11Device& device, LPCTSTR file_name, LPCSTR vertex_shader, 
			   LPCSTR pixel_shader) 
			   : vertex_shader_(NULL), pixel_shader_(NULL)
{
	if(vertex_shader)
	{
		ID3D10Blob *vertex_shader_blob;
		ID3D10Blob *error_blob;
		D3DX11CompileFromFile(file_name, 0, 0, vertex_shader, "vs_4_0", 0, 
							  0, 0, &vertex_shader_blob, &error_blob, NULL);
//		char* str = (char*)error_blob->GetBufferPointer();
		HRESULT hr = device.CreateVertexShader(vertex_shader_blob->GetBufferPointer(), 
								   vertex_shader_blob->GetBufferSize(), NULL,
								   &vertex_shader_);
		
		D3D11_INPUT_ELEMENT_DESC element_description[] = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, 
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		device.CreateInputLayout(element_description, 2, 
								 vertex_shader_blob->GetBufferPointer(), 
								 vertex_shader_blob->GetBufferSize(),
								 &input_layout_);
	}

	if(pixel_shader)
	{
		ID3D10Blob *pixel_shader_blob;

		D3DX11CompileFromFile(file_name, 0, 0, pixel_shader, "ps_4_0", 0, 0, 0, 
							  &pixel_shader_blob, 0, 0);
		device.CreatePixelShader(pixel_shader_blob->GetBufferPointer(),
								  pixel_shader_blob->GetBufferSize(), NULL,
								  &pixel_shader_);
	}
}

Shader::~Shader()
{
	if(vertex_shader_) vertex_shader_->Release();
	if(input_layout_) input_layout_->Release();
	if(pixel_shader_) pixel_shader_->Release();
}

void Shader::Activate(ID3D11DeviceContext& device_context)
{
	if(vertex_shader_) device_context.VSSetShader(vertex_shader_, 0, 0);
	if(pixel_shader_) device_context.PSSetShader(pixel_shader_, 0, 0);
	device_context.IASetInputLayout(input_layout_);
}