#include "../common.hlsl"

PS_IN vs_main(in VS_IN input)
{
    PS_IN output;

    float4 newPos = float4(input.pos, 1.0f);
	
    output.pos = newPos;

    // ‚t‚uÀ•W‚ğˆÚ“®‚³‚¹‚é
    float4 uv;
    uv.xy = input.tex; // s—ñŠ|‚¯Z‚Ì‚½‚ßflaot4Œ^‚ÉˆÚ‚·
    uv.z = 0.0f;
    uv.w = 1.0f;
    output.tex = uv.xy; // Š|‚¯Z‚ÌŒ‹‰Ê‚ğ‘—M—p•Ï”‚ÉƒZƒbƒg

    output.col = input.col;
    
    return output;
}