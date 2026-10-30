// Simple shader that takes positions and can add color

// need to know what information is in a vertex
// vertex shader input
struct VS_INPUT
{
    float3 position : POSITION;
};

// need a resulting set of data after process the vertex
struct VS_OUTPUT
{
    float4 position : SV_Position;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    return output;

}

// need a function to apply the color of the pixel
// pixel shader
float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(1, 1, 0, 1);

}