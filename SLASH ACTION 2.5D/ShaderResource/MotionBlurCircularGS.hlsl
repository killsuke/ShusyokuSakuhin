#include "common.hlsl"

float4 QuaternionSlerp(float4 q0, float4 q1, float t);
float3 RotateByQuaternion(float3 v, float4 q);

[maxvertexcount(64)]
void gs_main(point PS_IN input[1], inout TriangleStream<PS_IN> stream)
{
    matrix viewProj = View3D * Projection3D;

    float3 localRoot = float3(0, 0, 0);
    float3 localTip = float3(0, 0, length);

    for (int i = 0; i < shellCount; i++)
    {
        float t0 = (float) i / shellCount;
        float t1 = (float) (i + 1) / shellCount;

        float4 q0 = QuaternionSlerp(prevRot, currentRot, t0);
        float4 q1 = QuaternionSlerp(prevRot, currentRot, t1);

        float3 r0 = centerPos + RotateByQuaternion(localRoot, q0);
        float3 t0p = centerPos + RotateByQuaternion(localTip, q0);

        float3 r1 = centerPos + RotateByQuaternion(localRoot, q1);
        float3 t1p = centerPos + RotateByQuaternion(localTip, q1);

        PS_IN v;

        v.col = input[0].col * vertexColor;
        v.tex = float2(0, t0);
        v.pos = mul(float4(r0, 1), viewProj);
        stream.Append(v);

        v.col = input[0].col * vertexColor;
        v.tex = float2(1, t0);
        v.pos = mul(float4(t0p, 1), viewProj);
        stream.Append(v);

        v.col = input[0].col * vertexColor;
        v.tex = float2(0, t1);
        v.pos = mul(float4(r1, 1), viewProj);
        stream.Append(v);

        v.col = input[0].col * vertexColor;
        v.tex = float2(1, t1);
        v.pos = mul(float4(t1p, 1), viewProj);
        stream.Append(v);

        stream.RestartStrip();
    }
}

float4 QuaternionSlerp(float4 q0, float4 q1, float t)
{
    float dotNum = dot(q0, q1);
    
    if (dotNum < 0.0f)
    {
        q1 = -q1;
        dotNum = -dotNum;
    }
    
    if (dotNum > 0.9995f)
    {
        return normalize(lerp(q0, q1, t));
    }
    
    float theta = acos(dotNum);
    float sinTheta = sin(theta);
    
    return (sin((1 - t) * theta) / sinTheta) * q0 + (sin(t * theta) / sinTheta) * q1;
}

float3 RotateByQuaternion(float3 v, float4 q)
{
    float3 t = 2.0f * cross(q.xyz, v);
    return v + q.w * t + cross(q.xyz, t);
}