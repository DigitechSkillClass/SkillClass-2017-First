//--------------------------------------------------------------------------------------
// File: SimpleSample.fx
//
// The effect file for the SimpleSample sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
texture g_MeshTexture;              // Color texture for mesh
float g_fAngle;
//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler =
sampler_state
{
	Texture = <g_MeshTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
};

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
	float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
// color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS(float2 TextureUV  : TEXCOORD0)
{
	PS_OUTPUT Output;
	float fSin = sin(g_fAngle + TextureUV.y) * 0.15f;
	// Lookup mesh texture and modulate it with diffuse
	Output.RGBColor = tex2D(MeshTextureSampler, float2(TextureUV.x + fSin, TextureUV.y));
	//Output.RGBColor.a = 0.3f;
	//float f = (Output.RGBColor.r + Output.RGBColor.g + Output.RGBColor.b) / 3.0f;
	/*
	if (f < 0.001f)
	{
		Output.RGBColor.rgba = float4(f, f, f, 1.0f);
	}
	else
	{
		Output.RGBColor.rgba = float4(f, f, f, 0.0f);
	}
	*/
	//Output.RGBColor = float4(0.0f, 0.0f, 1.0f, 1.0f);
	return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene 
//--------------------------------------------------------------------------------------
technique RenderScene
{
	pass P0
	{
		//VertexShader = compile vs_2_0 RenderSceneVS();
		PixelShader = compile ps_2_0 RenderScenePS();
	}
}
