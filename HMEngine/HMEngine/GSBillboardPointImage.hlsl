#include "Common.hlsli"
#include "BillboardPointImage.hlsli"

[maxvertexcount(4)]
void main(
	point BillboardPI_GSInput i[1],
	inout TriangleStream< BillboardPI_PSInput > output
)
{
	float halfwidth = 0.5f;
	float halfheight = 0.5f;
	float4 worldPos = float4(0, 0, 0, 1);
	float3 billboard_front = normalize(eyeWorld - i[0].posWorld.xyz);
	float3 billboard_up = normalize(float3(0, 1, 0) - billboard_front * dot(billboard_front, float3(0, 1, 0)));
	float3 billboard_right = cross(billboard_up, billboard_front);

	BillboardPI_PSInput o;
	worldPos.xyz = i[0].posWorld.xyz + billboard_right * halfwidth + billboard_up * halfheight;
	o.posProj = mul(worldPos, viewProj);
	o.uv = float2(0, 0);
	output.Append(o);

	worldPos.xyz = i[0].posWorld.xyz - billboard_right * halfwidth + billboard_up * halfheight;
	o.posProj = mul(worldPos, viewProj);
	o.uv = float2(1, 0);
	output.Append(o);

	worldPos.xyz = i[0].posWorld.xyz + billboard_right * halfwidth - billboard_up * halfheight;
	o.posProj = mul(worldPos, viewProj);
	o.uv = float2(0, 1);
	output.Append(o);

	worldPos.xyz = i[0].posWorld.xyz - billboard_right * halfwidth - billboard_up * halfheight;
	o.posProj = mul(worldPos, viewProj);
	o.uv = float2(1, 1);
	output.Append(o);

}