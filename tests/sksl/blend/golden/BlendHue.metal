#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
struct Inputs {
    float4 src;
    float4 dst;
};
struct Outputs {
    float4 sk_FragColor [[color(0)]];
};


float _blend_color_luminance(float3 color) {
    return dot(float3(0.30000001192092896, 0.5899999737739563, 0.10999999940395355), color);
}
float3 _blend_set_color_luminance(float3 hueSatColor, float alpha, float3 lumColor) {
    float _0_blend_color_luminance;
    {
        _0_blend_color_luminance = dot(float3(0.30000001192092896, 0.5899999737739563, 0.10999999940395355), lumColor);
    }
    float lum = _0_blend_color_luminance;

    float _1_blend_color_luminance;
    {
        _1_blend_color_luminance = dot(float3(0.30000001192092896, 0.5899999737739563, 0.10999999940395355), hueSatColor);
    }
    float3 result = (lum - _1_blend_color_luminance) + hueSatColor;

    float minComp = min(min(result.x, result.y), result.z);
    float maxComp = max(max(result.x, result.y), result.z);
    if (minComp < 0.0 && lum != minComp) {
        result = lum + ((result - lum) * lum) / (lum - minComp);
    }
    return maxComp > alpha && maxComp != lum ? lum + ((result - lum) * (alpha - lum)) / (maxComp - lum) : result;
}
float _blend_color_saturation(float3 color) {
    return max(max(color.x, color.y), color.z) - min(min(color.x, color.y), color.z);
}
float3 _blend_set_color_saturation_helper(float3 minMidMax, float sat) {
    return minMidMax.x < minMidMax.z ? float3(0.0, (sat * (minMidMax.y - minMidMax.x)) / (minMidMax.z - minMidMax.x), sat) : float3(0.0);
}
float3 _blend_set_color_saturation(float3 hueLumColor, float3 satColor) {
    float _2_blend_color_saturation;
    {
        _2_blend_color_saturation = max(max(satColor.x, satColor.y), satColor.z) - min(min(satColor.x, satColor.y), satColor.z);
    }
    float sat = _2_blend_color_saturation;

    if (hueLumColor.x <= hueLumColor.y) {
        if (hueLumColor.y <= hueLumColor.z) {
            float3 _3_blend_set_color_saturation_helper;
            {
                _3_blend_set_color_saturation_helper = hueLumColor.x < hueLumColor.z ? float3(0.0, (sat * (hueLumColor.y - hueLumColor.x)) / (hueLumColor.z - hueLumColor.x), sat) : float3(0.0);
            }
            hueLumColor.xyz = _3_blend_set_color_saturation_helper;

        } else if (hueLumColor.x <= hueLumColor.z) {
            float3 _4_blend_set_color_saturation_helper;
            {
                _4_blend_set_color_saturation_helper = hueLumColor.x < hueLumColor.y ? float3(0.0, (sat * (hueLumColor.z - hueLumColor.x)) / (hueLumColor.y - hueLumColor.x), sat) : float3(0.0);
            }
            hueLumColor.xzy = _4_blend_set_color_saturation_helper;

        } else {
            float3 _5_blend_set_color_saturation_helper;
            {
                _5_blend_set_color_saturation_helper = hueLumColor.z < hueLumColor.y ? float3(0.0, (sat * (hueLumColor.x - hueLumColor.z)) / (hueLumColor.y - hueLumColor.z), sat) : float3(0.0);
            }
            hueLumColor.zxy = _5_blend_set_color_saturation_helper;

        }
    } else if (hueLumColor.x <= hueLumColor.z) {
        float3 _6_blend_set_color_saturation_helper;
        {
            _6_blend_set_color_saturation_helper = hueLumColor.y < hueLumColor.z ? float3(0.0, (sat * (hueLumColor.x - hueLumColor.y)) / (hueLumColor.z - hueLumColor.y), sat) : float3(0.0);
        }
        hueLumColor.yxz = _6_blend_set_color_saturation_helper;

    } else if (hueLumColor.y <= hueLumColor.z) {
        float3 _7_blend_set_color_saturation_helper;
        {
            _7_blend_set_color_saturation_helper = hueLumColor.y < hueLumColor.x ? float3(0.0, (sat * (hueLumColor.z - hueLumColor.y)) / (hueLumColor.x - hueLumColor.y), sat) : float3(0.0);
        }
        hueLumColor.yzx = _7_blend_set_color_saturation_helper;

    } else {
        float3 _8_blend_set_color_saturation_helper;
        {
            _8_blend_set_color_saturation_helper = hueLumColor.z < hueLumColor.x ? float3(0.0, (sat * (hueLumColor.y - hueLumColor.z)) / (hueLumColor.x - hueLumColor.z), sat) : float3(0.0);
        }
        hueLumColor.zyx = _8_blend_set_color_saturation_helper;

    }
    return hueLumColor;
}
float4 blend_hue(float4 src, float4 dst) {
    float alpha = dst.w * src.w;
    float3 sda = src.xyz * dst.w;
    float3 dsa = dst.xyz * src.w;
    return float4((((_blend_set_color_luminance(_blend_set_color_saturation(sda, dsa), alpha, dsa) + dst.xyz) - dsa) + src.xyz) - sda, (src.w + dst.w) - alpha);
}
fragment Outputs fragmentMain(Inputs _in [[stage_in]], bool _frontFacing [[front_facing]], float4 _fragCoord [[position]]) {
    Outputs _outputStruct;
    thread Outputs* _out = &_outputStruct;
    float4 _9_blend_hue;
    {
        float _10_alpha = _in.dst.w * _in.src.w;
        float3 _11_sda = _in.src.xyz * _in.dst.w;
        float3 _12_dsa = _in.dst.xyz * _in.src.w;
        _9_blend_hue = float4((((_blend_set_color_luminance(_blend_set_color_saturation(_11_sda, _12_dsa), _10_alpha, _12_dsa) + _in.dst.xyz) - _12_dsa) + _in.src.xyz) - _11_sda, (_in.src.w + _in.dst.w) - _10_alpha);
    }
    _out->sk_FragColor = _9_blend_hue;

    return *_out;
}
