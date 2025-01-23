#include "Common.hlsli"
#include "Billboard.hlsli"

[maxvertexcount(4)]
void main(
	point Billboard_GSInput i[1], 
	inout TriangleStream< Billboard_PSInput > output
)
{
	float halfwidth = 1;
	float halfheight = 1;
	float4 worldPos = float4(0, 0, 0, 1);
	float3 billboard_right = -float3(view[0][0], view[0][1], view[0][2]);
	float3 billboard_up = float3(view[1][0], view[1][1], view[1][2]);
	float3 billboard_front = -float3(view[2][0], view[2][1], view[2][2]);
	
	Billboard_PSInput o;
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