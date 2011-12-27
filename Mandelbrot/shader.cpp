#include "shader.h"
#include "shader_parameters.h"

Shader::Shader(ID3D11Device& device, LPCTSTR file_name, LPCSTR vertex_shader, 
			   LPCSTR pixel_shader, LPCSTR texture) 
			   : vertex_shader_(NULL), pixel_shader_(NULL), input_variables_(NULL)
{
	if(vertex_shader)
	{
		ID3D10Blob *vertex_shader_blob;
		ID3D10Blob *error_blob;
		D3DX11CompileFromFile(file_name, 0, 0, vertex_shader, "vs_4_0", 0, 
							  0, 0, &vertex_shader_blob, &error_blob, NULL);
		char *str;
		if(error_blob)
			str = (char*)error_blob->GetBufferPointer();
		HRESULT hr = device.CreateVertexShader(vertex_shader_blob->GetBufferPointer(), 
								   vertex_shader_blob->GetBufferSize(), NULL,
								   &vertex_shader_);

		
		D3D11_INPUT_ELEMENT_DESC element_description[] = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, 
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, 
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		device.CreateInputLayout(element_description, 3, 
								 vertex_shader_blob->GetBufferPointer(), 
								 vertex_shader_blob->GetBufferSize(),
								 &input_layout_);
		if(error_blob) error_blob->Release();
		vertex_shader_blob->Release();
	}

	if(pixel_shader)
	{
		ID3D10Blob *pixel_shader_blob;

		D3DX11CompileFromFile(file_name, 0, 0, pixel_shader, "ps_4_0", 0, 0, 0, 
							  &pixel_shader_blob, 0, 0);
		device.CreatePixelShader(pixel_shader_blob->GetBufferPointer(),
								  pixel_shader_blob->GetBufferSize(), NULL,
								  &pixel_shader_);

		pixel_shader_blob->Release();
	}

	if(texture)
	{
		D3DX11CreateShaderResourceViewFromFile(&device, texture, NULL, NULL, &texture_, 
											   NULL);
		D3D11_SAMPLER_DESC sampler_description;
		ZeroMemory(&sampler_description, sizeof(D3D11_SAMPLER_DESC));

		sampler_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_description.MaxAnisotropy = 1;
		sampler_description.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_description.MaxLOD = D3D11_FLOAT32_MAX;
		device.CreateSamplerState(&sampler_description, &sampler_state_);
	}
}

Shader::~Shader()
{
	if(vertex_shader_) vertex_shader_->Release();
	if(input_layout_) input_layout_->Release();
	if(pixel_shader_) pixel_shader_->Release();
	if(texture_) texture_->Release();
	if(sampler_state_) sampler_state_->Release();
	if(input_variables_) delete input_variables_;
}

void Shader::Activate(ID3D11DeviceContext& device_context)
{
	if(vertex_shader_) device_context.VSSetShader(vertex_shader_, 0, 0);
	if(pixel_shader_) device_context.PSSetShader(pixel_shader_, 0, 0);
	device_context.IASetInputLayout(input_layout_);
	device_context.PSSetSamplers(0, 1, &sampler_state_);
	device_context.PSSetShaderResources(0, 1, &texture_);

	if(input_variables_)
	{
		device_context.PSSetConstantBuffers(0, 1, input_variables_->GetBuffer());
		device_context.VSSetConstantBuffers(0, 1, input_variables_->GetBuffer());
	}
}