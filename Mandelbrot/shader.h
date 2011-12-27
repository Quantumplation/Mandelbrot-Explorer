#ifndef SHADER_H
#define SHADER_H

#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

class ShaderParameters;

class Shader
{
public:
	Shader(ID3D11Device& device, LPCTSTR file_name, LPCSTR vertex_shader_name, 
		   LPCSTR pixel_shader_name, LPCSTR texture);
	~Shader();

	template<typename T>
	void SetParameters(const T& parameters, ID3D11DeviceContext& device_context)
	{
		if(input_variables_ == NULL)
		{
			D3D11_BUFFER_DESC buffer_description;
			ZeroMemory(&buffer_description, sizeof(D3D11_BUFFER_DESC));
			buffer_description.ByteWidth = sizeof(T);
			buffer_description.Usage = D3D11_USAGE_DYNAMIC;
			buffer_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			ID3D11Device* device(NULL);
			device_context.GetDevice(&device);

			input_variables_ = new ShaderParameters(*device, buffer_description);
		}
		input_variables_->SetData<T>(parameters, device_context);
	}

	void Activate(ID3D11DeviceContext& device_context);
private:
	ID3D11VertexShader *vertex_shader_;
	ID3D11PixelShader  *pixel_shader_;
	ID3D11ShaderResourceView* texture_;
	ID3D11SamplerState* sampler_state_;
	ID3D11InputLayout *input_layout_;

	ShaderParameters *input_variables_;
};

#endif