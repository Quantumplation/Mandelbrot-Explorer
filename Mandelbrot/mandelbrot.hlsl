Texture2D gradient;
SamplerState SampleType;

cbuffer Mandelbrot
{
	float scale;
	float2 center;
}

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

VOut VShader(VertexInputType input)
{
	VOut output;

	output.position = input.position;
	output.tex = input.tex;
	output.color = input.color;

	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
	float2 z, c;
	c.x = (input.tex.r * 2 - 1) * scale + center.x;
	c.y = (input.tex.g * 2 - 1) * scale + center.y;

	z = c;
	float i;
	for(i = 0; i<500; i++)
	{
		float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;
		z.x = x;
		z.y = y;
		if((x * x + y * y) > 20.0*20.0) break;
	}

	float modulus = sqrt(z.x * z.x + z.y * z.y);
	if(modulus < 20)
		input.color.rgb = 0;
	else
	{
		// Renormalize for smooth color falloff
		float mu = (i) - (log(log(modulus))) / log(2);
		input.color.rgb = gradient.Sample(SampleType, mu/130, 0);
	}
	return input.color;
}