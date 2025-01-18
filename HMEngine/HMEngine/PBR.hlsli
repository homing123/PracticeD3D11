
//#include "Common.hlsli"
//
//Texture2D tex0 : register(t0);
//SamplerState texSamp0 : register(s0);
//TextureCube texCubeDiffuse : register(t1);
//TextureCube texCubeSpecular : register(t2);
//
//cbuffer PSCBuffer : register(b0)
//{
//	float3 eyeWorld;
//	int useTexture;
//	Material mat;
//	Light light[MAX_LIGHTS];
//	float3 rimColor;
//	float rimPower;
//	float rimStrength;
//	float3 dummy;
//}
//
//float4 main(PSInput input) : SV_TARGET
//{
//	float3 view = normalize(eyeWorld - input.posWorld);
//
//    //float3 diffuse = texCubeDiffuse.Sample(texSamp0, input.normalWorld) * mat.diffuse;
//    //float3 specular = texCubeSpecular.Sample(texSamp0, reflect(-view, input.normalWorld)) * mat.specular;
//    //return float4(diffuse + specular, 1);
//
//	float3 color = float3(0,0,0);
//	int i = 0;
//
//    [unroll]
//    for (i = 0; i < NUM_DIR_LIGHTS; ++i)
//    {
//        color += ComputeDirectionalLight(light[i], mat, input.normalWorld, view);
//    }
//    [unroll]
//    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
//    {
//        color += ComputePointLight(light[i], mat, input.posWorld, input.normalWorld, view);
//    }
//
//    [unroll]
//    for (i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
//    {
//        color += ComputeSpotLight(light[i], mat, input.posWorld, input.normalWorld, view);
//    }
//
//    float rim = 1 - max(0, dot(view, input.normalWorld));
//    rim = pow(rim, rimPower);
//    rim = smoothstep(0, 1, rim);
//    color += rim * rimStrength * rimColor;
//
//    return useTexture ? float4(color, 1.0) * tex0.Sample(texSamp0, input.uv) : float4(color, 1.0);
//
//  
//
// 
//}