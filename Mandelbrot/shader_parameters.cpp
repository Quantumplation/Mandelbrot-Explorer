#include "shader_parameters.h"

ShaderParameters::ShaderParameters(ID3D11Device& device, const D3D11_BUFFER_DESC& buffer_description)
{
	device.CreateBuffer(&buffer_description, NULL, &buffer_);
}

ShaderParameters::~ShaderParameters()
{
	buffer_->Release();
}

ID3D11Buffer* const* ShaderParameters::GetBuffer() const
{
	return &buffer_;
}