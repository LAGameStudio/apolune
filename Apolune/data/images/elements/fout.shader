float4 vec4(float x0, float x1, float x2, float x3)
{
    return float4(x0, x1, x2, x3);
}
// Varyings
static float2 _v_vTexcoord = {0, 0};

static float4 gl_Color[1] =
{
    float4(0, 0, 0, 0)
};


uniform float _gm_AlphaRefValue : register(c3);
uniform bool _gm_AlphaTestEnabled : register(c4);
uniform sampler2D _gm_BaseTexture : register(s0);
uniform float4 _gm_FogColour : register(c5);
uniform bool _gm_PS_FogEnabled : register(c6);

float4 gl_texture2D(sampler2D s, float2 t)
{
    return tex2D(s, t);
}

#define GL_USES_FRAG_COLOR
;
;
;
;
;
void _DoAlphaTest(in float4 _SrcColour)
{
{
if(_gm_AlphaTestEnabled)
{
{
if((_SrcColour.w <= _gm_AlphaRefValue))
{
{
discard;
;
}
;
}
;
}
;
}
;
}
}
;
void _DoFog(inout float4 _SrcColour, in float _fogval)
{
{
if(_gm_PS_FogEnabled)
{
{
(_SrcColour = lerp(_SrcColour, _gm_FogColour, clamp(_fogval, 0.0, 1.0)));
}
;
}
;
}
}
;
;
;
void gl_main()
{
{
float4 _t1 = gl_texture2D(_gm_BaseTexture, _v_vTexcoord);
float _c = (1.0 - _t1.w);
(gl_Color[0] = vec4(_c, _c, _c, _c));
}
}
;
struct PS_INPUT
{
    float2 v0 : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 gl_Color0 : COLOR0;
};

PS_OUTPUT main(PS_INPUT input)
{
    _v_vTexcoord = input.v0.xy;

    gl_main();

    PS_OUTPUT output;
    output.gl_Color0 = gl_Color[0];

    return output;
}
