#include "common.hlsl"

PS_IN vs_main(in VS_IN input)
{
    PS_IN output;
    
    float4 newPos = float4(input.pos * Expand, 1.0f);
	
    output.pos = mul(newPos, matrixWorld);

    output.pos = mul(output.pos, View3D);

    output.pos = mul(output.pos, Projection3D);

	// ‚t‚uÀ•W‚ğˆÚ“®‚³‚¹‚é
    float4 uv;
    uv.xy = input.tex; // s—ñŠ|‚¯Z‚Ì‚½‚ßflaot4Œ^‚ÉˆÚ‚·
    uv.z = 0.0f;
    uv.w = 1.0f;
	//uv = mul(uv, matrixTex1);	// ‚t‚uÀ•W‚ÆˆÚ“®s—ñ‚ğŠ|‚¯Z
    output.tex = uv.xy; // Š|‚¯Z‚ÌŒ‹‰Ê‚ğ‘—M—p•Ï”‚ÉƒZƒbƒg

    output.tex = (output.tex - 0.5f) / Expand + 0.5f;
    
    output.col = input.col * vertexColor;

    return output;
}