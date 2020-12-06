#include "Constants.hlsl"

struct VIn
{
	float3 position : POSITION;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float2 TexCoords : TEXCOORD0;
};

struct VOut
{
	float4 position : SV_POSITION;
	float2 TexCoords : TEXCOORD0;
	float4 normal : NORMAL0;
	float4 WorldPos : POSITION0;
	float4 tangent : TANGENT0;
};

VOut VS(VIn vIn)
{
	VOut output;

	// transform input position from model to world space
	output.position = mul(float4(vIn.position, 1.0f), c_modelToWorld);
	output.WorldPos = output.position;
	// transform position from world to projection space
	output.position = mul(output.position, c_viewProj);
	output.TexCoords = vIn.TexCoords;
	//Light
	output.normal = mul(float4(vIn.normal, 0.0f), c_modelToWorld);
	output.tangent = mul(float4(vIn.tangent, 0.0f), c_modelToWorld);

	return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
	//Create TBN Matrix 
	float3 t = normalize(pIn.tangent.xyz);
	float3 n = normalize(pIn.normal.xyz);
	float3 b = normalize(cross(n,t));
	float3x3 TBN = float3x3(t.x , t.y, t.z,
					b.x, b.y, b.z,
					n.x, n.y, n.z );
	//Unbias Sample Texture
	float4 texCol = NormalMapTex.Sample(DefaultSampler, pIn.TexCoords);
	float4 sampled = (texCol * 2.0) - float4(1.0, 1.0, 1.0, 1.0);
	float3 surfN = mul(sampled.xyz, TBN);

	//Lighting Calculations
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
			float NFactor = mul(surfN, l);
			NFactor = max(NFactor, 0.0f);
			LightCol += NFactor * c_pointLight[i].diffuseColor * falloff;
			//Specular Color Term
			float3 r = normalize(reflect(-l, surfN));
			float3 v = normalize(c_cameraposition - pIn.WorldPos);
			float ReflectiveFactor = mul(r , v);
			ReflectiveFactor = max(ReflectiveFactor, 0.0f);
			float SpecularPowerView = pow(ReflectiveFactor, c_pointLight[i].specularPower);
			LightCol += c_pointLight[i].specularColor * SpecularPowerView * falloff;
		}
	}
	float4 Lights = float4(LightCol, 1.0f);
	return Lights;
}