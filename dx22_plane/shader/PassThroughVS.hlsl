#include "common.hlsl"

PS_IN vs_main(in VS_IN input)
{
    PS_IN output;
    
    output.pos = float4(input.pos, 1.0f);
    output.col = input.col;
    output.tex = input.tex;
    
    return output;
}