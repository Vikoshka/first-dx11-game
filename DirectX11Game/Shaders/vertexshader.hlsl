cbuffer m_inData : register (b0)
{
    float2 origin;
    float2 translation;
    float2 scale;
    float rotation;
    float depth;
}

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float2 resultPos = input.inPos;
    
    // First translation (origin)
    resultPos -= origin;
    
    // Apply rotation
    float2x2 rotationMatrix2D = float2x2
    (
        cos(rotation),  sin(rotation), 
        -sin(rotation), cos(rotation)
    );
    resultPos = mul(resultPos, rotationMatrix2D);
    
    // Apply scale
    float2x2 scaleMatrix2x2 = float2x2
    (
        scale.x,        0.0f, 
        0.0f,           scale.y
    );
    resultPos = mul(resultPos, scaleMatrix2x2);
    
    // Apply transform and translation
    resultPos += translation;
    
    // Combine output
    output.outPos = float4(resultPos, depth, 1.0f);
    output.outTexCoord = input.inTexCoord;
    return output;
}

