#ifndef SPRITE_H
#define SPRITE_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

struct Vertex
{
	float X, Y, Z;
	D3DXCOLOR Color;
	float U, V;
};

class Sprite
{
public:
	Sprite(ID3D11Device& device, ID3D11DeviceContext& context);
	~Sprite();

	void Activate(ID3D11DeviceContext& device_context);
	void Draw(ID3D11DeviceContext& device_context);
private:
	ID3D11Buffer *vertex_buffer_;
};

#endif