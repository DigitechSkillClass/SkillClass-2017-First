float3 fvLightPosition = float3( -100.00, 100.00, -100.00 );
float3 fvEyePosition = float3( 0.00, 0.00, -100.00 );
float4x4 matWorld;
float4x4 matView;
float4x4 matViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0;
};

struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection :  TEXCOORD2;
   float3 Normal :          TEXCOORD3;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position         = mul( Input.Position, matWorld );
   Output.Position         = mul( Output.Position, matViewProjection );
   Output.Texcoord         = Input.Texcoord;
   
   float3 fvObjectPosition = mul( Input.Position, matWorld );
   
   Output.ViewDirection    = fvEyePosition - fvObjectPosition;
   Output.LightDirection   = fvLightPosition - fvObjectPosition;
   Output.Normal           = mul( Input.Normal, matWorld );
      
   return( Output );
}



float4 fvAmbient = float4( 0.37, 0.37, 0.37, 1.00 );
float4 fvSpecular = float4( 0.49, 0.49, 0.49, 1.00 );
float4 fvDiffuse = float4( 0.89, 0.89, 0.89, 1.00 );
float fSpecularPower = float( 25.00 );
texture splat1;
sampler2D splatMap1 = sampler_state
{
   Texture = (splat1);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture splat2;
sampler2D splatMap2 = sampler_state
{
	Texture = (splat2);
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};
texture splat3;
sampler2D splatMap3 = sampler_state
{
	Texture = (splat3);
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};
texture splat4;
sampler2D splatMap4 = sampler_state
{
	Texture = (splat4);
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};
texture splatData;
sampler2D splatDataMap = sampler_state
{
	Texture = (splatData);
	ADDRESSU = WRAP;
	ADDRESSV = WRAP;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection:   TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   
};

float4 ps_main( PS_INPUT Input ) : COLOR0
{      
   float3 fvLightDirection = normalize( Input.LightDirection );
   float3 fvNormal         = normalize( Input.Normal );
   float  fNDotL           = dot( fvNormal, fvLightDirection ); 
   
   float3 fvReflection     = normalize(reflect(-fvLightDirection, fvNormal)); 
   float3 fvViewDirection  = normalize( Input.ViewDirection );
   float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );
   
   float4 fvSplat1 = tex2D(splatMap1, Input.Texcoord);
   float4 fvSplat2 = tex2D(splatMap2, Input.Texcoord );
   float4 fvSplat3 = tex2D(splatMap3, Input.Texcoord);
   float4 fvSplat4 = tex2D(splatMap4, Input.Texcoord);
   float4 fvSplatData = tex2D(splatDataMap, Input.Texcoord);
   float4 fvBaseColor = fvSplatData.r * fvSplat1 + fvSplatData.g * fvSplat2 + fvSplatData.b * fvSplat3 + fvSplatData.a * fvSplat4;

   float4 fvTotalAmbient   = fvAmbient * fvBaseColor; 
   float4 fvTotalDiffuse   = fvDiffuse * fNDotL * fvBaseColor; 
   float4 fvTotalSpecular  = fvSpecular * pow( fRDotV, fSpecularPower );
   
   return( saturate( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular ) );
      
}

//--------------------------------------------------------------//
// Technique Section for Textured Phong
//--------------------------------------------------------------//
technique Textured_Phong
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 vs_main();
      PixelShader = compile ps_2_0 ps_main();
   }
}

