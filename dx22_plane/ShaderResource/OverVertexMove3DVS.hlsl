#include "common.hlsl"

VS_OUTPUT vs_main(in VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    float4 newPos = float4(input.pos, 1.0f);
    
    // ã‘¤‚Ì’¸“_‚¾‚¯“®‚©‚·i‚t‚u‚Ì‚u‚Ì’l‚ª‚O‚Ì’¸“_j
    if (input.tex.y == 0.0f)
    {
        newPos.y += baseScale * currentScale; // ’l•ª‚¾‚¯’·‚­‚·‚é
    }
    
    float4 worldPos = mul(newPos, matrixWorld);
    float4 viewPos = mul(worldPos, View3D);
    output.pos = mul(viewPos, Projection3D);

	// ‚t‚uÀ•W‚ğˆÚ“®‚³‚¹‚é
    float4 uv;
    uv.xy = input.tex; // s—ñŠ|‚¯Z‚Ì‚½‚ßflaot4Œ^‚ÉˆÚ‚·
    if (isInvertX == false)
    {
        uv.x = 1.0f - uv.x; // ‚t‚uÀ•W‚Ì‚w‚ğ”½“]
    }
    uv.z = 0.0f;
    uv.w = 1.0f;
    uv = mul(uv, matrixTex); // ‚t‚uÀ•W‚ÆˆÚ“®s—ñ‚ğŠ|‚¯Z
    output.tex = uv.xy; // Š|‚¯Z‚ÌŒ‹‰Ê‚ğ‘—M—p•Ï”‚ÉƒZƒbƒg
        
    output.col = input.col * vertexColor;
    
    return output;
}