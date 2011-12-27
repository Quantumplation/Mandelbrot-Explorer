#ifndef SHADER_PARAMETERS_H
#define SHADER_PARAMETERS_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

class ShaderParameters
{
public:
	ShaderParameters(ID3D11Device& device, const D3D11_BUFFER_DESC& buffer_description);
	~ShaderParameters();

	ID3D11Buffer* const* GetBuffer() const;

	template<typename T>
	void SetData(const T& data, ID3D11DeviceContext& device_context)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
		device_context.Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);

		T* data_pointer = (T*)mapped_subresource.pData;
		memcpy(data_pointer, &data, sizeof(T));

		device_context.Unmap(buffer_, 0);
	}
private:
	ID3D11Buffer *buffer_;
};

#endif