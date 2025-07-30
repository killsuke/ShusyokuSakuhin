#include "shader/common.hlsl"

float4 ps_main(in VS_OUTPUT input) : SV_TARGET
{
    return input.col;
}