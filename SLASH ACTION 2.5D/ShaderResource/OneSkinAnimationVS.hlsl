#include "common.hlsl"

VS_OUTPUT vs_main(in VS_ANIMATION input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    float4 w = input.weight;

    // 安全策として重みを正規化
    float weightSum = w.x + w.y + w.z + w.w;
    if (weightSum > 0.0f)
    {
        w /= weightSum;
    }
    
    float4x4 comb = (float4x4) 0;
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        comb += bonecombmtx[input.idx[i]] * w[i];
    }
    
   // comb += bonecombmtx[input.idx[3]] * (1.0f - w[0] - w[1] - w[2]);
        
    float4 localPos = float4(input.pos.xyz, 1.0f);
    
    // 先にワールド行列をかける、そうじゃないとサイズのずれが発生する
    // ポジション × コンビネーション行列は実質ワールド行列であるため
    output.pos = mul(localPos, matrixWorldB);
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