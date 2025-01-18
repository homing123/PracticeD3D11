//#include "Common.hlsli"
//
//TextureCube texCube0 : register(t0);
//SamplerState texSamp : register(s0);
//
//float4 main(PSInput input) : SV_TARGET
//{
//	return texCube0.Sample(texSamp, input.posWorld.xyz);
//}