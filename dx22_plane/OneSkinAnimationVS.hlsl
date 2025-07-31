#include "shader/common.hlsl"

VS_OUTPUT vs_main(in VS_ANIMATION input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    float w[3] = (float[3]) input.weight;

    float4x4 comb = (float4x4) 0;

    for (int i = 0; i < 3; i++)
    {
        comb += bonecombmtx[input.idx[i]] * w[i];
    }
    
    comb += bonecombmtx[input.idx[3]] * (1.0f - w[0] - w[1] - w[2]);
        
    output.pos = mul(float4(input.pos.xyz, 1.0f), matrixWorldB);
   // output.pos = mul(input.pos, comb);
    //output.pos = float4(0.0f, 0.0f, 0.0f, 1.0f);
//    output.Pos = mul(comb,Pos);

//    output.Pos = mul(Pos, World);
    //output.pos = float4(input.pos.xyz, 1.0f);
    output.pos = mul(output.pos, comb);
    output.wpos = output.pos;
    output.pos = mul(output.pos, View3D);
    output.pos = mul(output.pos, Projection3D);

    output.col = input.col;
  //  output.col = float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    return output;
}