#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
struct Uniforms {
    float4 sk_RTAdjust;
};
struct Inputs {
};
struct Outputs {
    float4 sk_Position [[position]];
    float sk_PointSize [[point_size]];
};

vertex Outputs vertexMain(Inputs _in [[stage_in]], constant Uniforms& _uniforms [[buffer(0)]], uint sk_VertexID [[vertex_id]], uint sk_InstanceID [[instance_id]]) {
    Outputs _outputStruct;
    thread Outputs* _out = &_outputStruct;
    _out->sk_Position = float4(1.0);
    _out->sk_Position = float4(_out->sk_Position.xy * _uniforms.sk_RTAdjust.xz + _out->sk_Position.ww * _uniforms.sk_RTAdjust.yw, 0.0, _out->sk_Position.w);
    _out->sk_Position.y = -_out->sk_Position.y;
    return *_out;
}
