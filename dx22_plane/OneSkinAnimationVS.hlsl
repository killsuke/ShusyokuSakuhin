#include "shader/common.hlsl"

VS_OUTPUT main(in VS_ANIMATION input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    float w[3] = (float[3]) input.weight;

    float4x4 comb = (float4x4) 0;

    for (int i = 0; i < 3; i++)
    {
        comb += bonecombmtx[input.idx[i]] * w[i];
    }
    
    comb += bonecombmtx[input.idx[3]] * (1.0f - w[0] - w[1] - w[2]);
    
    output.pos = mul(input.pos, comb);
//    output.Pos = mul(comb,Pos);

//    output.Pos = mul(Pos, World);
    output.wpos = output.pos;
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);

    output.col = (1.0f, 1.0f, 1.0f, 1.0f);
    
    return output;
}