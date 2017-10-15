float HBloomWeights[9];                 // Description of the sampling distribution used by
float HBloomOffsets[9];                 // the HorizontalBlur() function

float VBloomWeights[9];                 // Description of the sampling distribution used by
float VBloomOffsets[9];                 // the VerticalBlur() function

texture tex;
sampler2D tex0 = sampler_state
{
	Texture = (tex);
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

//------------------------------------------------------------------
// HORIZONTAL BLUR
//
// Takes 9 samples from the down-sampled texture (4 either side and
// one central) biased by the provided weights. Different weight
// distributions will give more subtle/pronounced blurring.
//------------------------------------------------------------------
float4 HorizontalBlur(in float2 t : TEXCOORD0) : COLOR
{
	float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < 9; i++)
	{
		color += (tex2D(tex0, t + float2(HBloomOffsets[i], 0.0f)) * HBloomWeights[i]);
	}
	
	return float4(color.rgb, 1.0f);
}

//------------------------------------------------------------------
// VERTICAL BLUR
//
// Takes 9 samples from the down-sampled texture (4 above/below and
// one central) biased by the provided weights. Different weight
// distributions will give more subtle/pronounced blurring.
//------------------------------------------------------------------
float4 VerticalBlur(in float2 t : TEXCOORD0) : COLOR
{
	float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < 9; i++)
	{
		color += (tex2D(tex0, t + float2(0.0f, VBloomOffsets[i])) * VBloomWeights[i]);
	}
	
	return float4(color.rgb, 1.0f);
}

struct POST_VS_INPUT
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
};

struct POST_VS_OUTPUT
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
};

POST_VS_OUTPUT post_vs(POST_VS_INPUT Input)
{
	POST_VS_OUTPUT Output;
	Output.Position = Input.Position;
	Output.Texcoord = Input.Texcoord;
	return Output;
}

float4 post_ps(in float2 t : TEXCOORD0) : COLOR0
{
	return (VerticalBlur(t) + HorizontalBlur(t)) / 2;
}

//--------------------------------------------------------------//
// Technique Section for Textured Phong
//--------------------------------------------------------------//
technique Textured_Phong
{
   pass Pass_0
   {
	   VertexShader = compile vs_3_0 post_vs();
	   PixelShader = compile ps_3_0 post_ps();
   }
}

