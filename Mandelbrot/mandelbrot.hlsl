struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = position;

	// Encode the position (between -1 and 1) into the color.
	output.color.r = (position.x + 1)/2;
	output.color.g = (position.y + 1)/2;
	output.color.ba = 0;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	float2 z, c;
	c.x = color.r * 2 - 1.5;
	c.y = color.g * 2 - 1;

	z = c;
	float i;
	for(i = 0; i<400; i++)
	{
		float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;
		z.x = x;
		z.y = y;
		if((x * x + y * y) > 20.0*20.0) break;
	}

	float modulus = sqrt(z.x * z.x + z.y * z.y);
	if(modulus < 20)
		color.rgb = 0;
	else
	{
		float mu = (i) - (log(log(modulus))) / log(2);
		color.rgb = mu / 30;
	}
	return color;
}