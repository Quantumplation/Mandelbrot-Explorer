#include "sprite.h"

Sprite::Sprite(ID3D11Device& device, ID3D11DeviceContext& device_context)
{
	Vertex vertex_data[] = 
	{
		{1.0f, 1.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1.0f, 1.0f},
		{1.0f, -1.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0.0f},
		{-1.0f, 1.0f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f},
		{-1.0f, 1.0f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f},
		{1.0f, -1.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0.0f},
		{-1.0f, -1.0f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, 0.0f},
	};

	D3D11_BUFFER_DESC buffer_description;
	ZeroMemory(&buffer_description, sizeof(D3D11_BUFFER_DESC));

	buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	buffer_description.ByteWidth = sizeof(Vertex) * 6;
	buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device.CreateBuffer(&buffer_description, NULL, &vertex_buffer_);

	D3D11_MAPPED_SUBRESOURCE mapped_buffer;
	device_context.Map(vertex_buffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_buffer);

	memcpy(mapped_buffer.pData, vertex_data, sizeof(Vertex) * 6);

	device_context.Unmap(vertex_buffer_, NULL);
}

Sprite::~Sprite()
{
	vertex_buffer_->Release();
}

void Sprite::Activate(ID3D11DeviceContext& device_context)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	device_context.IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
	device_context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Sprite::Draw(ID3D11DeviceContext& device_context)
{
	device_context.Draw(6, 0);
}