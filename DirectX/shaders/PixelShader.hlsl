cbuffer CBuf
{
	float3 faceColor[6];
};

float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return float4(faceColor[tid / 2], 1.0f);
}

