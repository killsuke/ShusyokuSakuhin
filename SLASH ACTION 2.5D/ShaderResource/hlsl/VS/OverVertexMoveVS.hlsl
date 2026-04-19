#include "../common.hlsl"

VS_OUTPUT vs_main(in VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    float4 newPos = float4(input.pos, 1.0f);
    
    // ã‘¤‚Ì’¸“_‚¾‚¯“®‚©‚·i‚t‚u‚Ì‚u‚Ì’l‚ª‚O‚Ì’¸“_j
    if (input.tex.y == 0.0f)
    {
        newPos.y += baseScale * currentScale; // HP‚Ì”‚¾‚¯’·‚­‚·‚é
    }
    
    float4 worldPos = mul(newPos, matrixWorld);
    float4 viewPos = mul(worldPos, View2D);
    output.pos = mul(viewPos, Projection2D);

	// ‚t‚uÀ•W‚ğˆÚ“®‚³‚¹‚é
    float4 uv;
    uv.xy = input.tex; // s—ñŠ|‚¯Z‚Ì‚½‚ßflaot4Œ^‚ÉˆÚ‚·
    uv.z = 0.0f;
    uv.w = 1.0f;

    output.tex = uv.xy; // Š|‚¯Z‚ÌŒ‹‰Ê‚ğ‘—M—p•Ï”‚ÉƒZƒbƒg
        
    output.col = input.col * vertexColor;
    
    return output;
}