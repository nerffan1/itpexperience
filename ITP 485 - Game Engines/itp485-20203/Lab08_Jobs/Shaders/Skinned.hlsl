#include "Constants.hlsl"

struct VIn
{
	float3 position : POSITION;
	float3 normal : NORMAL0;
	uint4 boneIndex : BONE0;
	float4 boneWeight : WEIGHT0;
	float2 uv : TEXCOORD0;
};

struct VOut
{
	float4 position : SV_POSITION;
	float2 TexCoords : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 WorldPos : POSITION0;
};

VOut VS(VIn vIn)
{
	VOut output;
	
	//Weigted Change
	for (int i = 0; i < 4; i++)
	{
		//Output Position
		float4 newPose = mul(float4(vIn.position, 1.0f), c_skinMatrix[vIn.boneIndex[i]]);
		newPose *= vIn.boneWeight[i];
		output.position += newPose;
		//Normal Change
		float4 newNormal = mul(float4(vIn.normal, 0.0f), c_skinMatrix[vIn.boneIndex[i]]);
		newNormal *= vIn.boneWeight[i];
		output.normal += newNormal;
	}
	//UV
	output.TexCoords = vIn.uv;
	//Normal
	output.normal = mul(float4(output.normal, 0.0f), c_modelToWorld).xyz;
	//Output Position
	output.position = mul(output.position, c_modelToWorld);
	output.WorldPos = output.position.xyz;
	output.position = mul(output.position, c_viewProj);
	return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
	float3 n = normalize(pIn.normal);
	float4 texCol = DiffuseTexture.Sample(DefaultSampler, pIn.TexCoords);
	float3 LightCol = c_ambient;
	float falloff = 0;
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if (c_pointLight[i].isEnabled)
		{
			//Falloff
			float d = distance(pIn.WorldPos , c_pointLight[i].position);
			float mind = c_pointLight[i].innerRadius;
			float maxd = c_pointLight[i].outerRadius;
			falloff = 1 - smoothstep(mind, maxd,d);

			//Diffuse Color Term
			float3 l = normalize(c_pointLight[i].position - pIn.WorldPos);
			float NFactor = mul(n, l);
			NFactor = max(NFactor, 0.0f);
			LightCol += NFactor * c_pointLight[i].diffuseColor * falloff;
			//Specular Color Term
			float3 r = normalize(reflect(-l, n));
			float3 v = normalize(c_cameraposition - pIn.WorldPos);
			float ReflectiveFactor = mul(r , v);
			ReflectiveFactor = max(ReflectiveFactor, 0.0f);
			float SpecularPowerView = pow(ReflectiveFactor, c_pointLight[i].specularPower);
			LightCol += c_pointLight[i].specularColor * SpecularPowerView * falloff;
		}
	}
	float4 Lights = float4(LightCol, 1.0f);
	return Lights * texCol;
}