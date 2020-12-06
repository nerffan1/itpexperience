#include "Constants.hlsl"

struct VIn
{
	float3 position : POSITION;
	float2 TexCoords : TEXCOORD0;
};

struct VOut
{
	float4 position : SV_POSITION;
	float2 TexCoords : TEXCOORD0;
};

VOut VS(VIn vIn)
{
	VOut output;

	// transform input position from model to world space
	output.position = mul(float4(vIn.position, 1.0f), c_modelToWorld);
	// transform position from world to projection space
	output.position = mul(output.position, c_viewProj);
	//Texture Coordinates 
	output.TexCoords = vIn.TexCoords;

	return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
	float4 texCol = DiffuseTexture.Sample(DefaultSampler, pIn.TexCoords);
	return texCol;
}