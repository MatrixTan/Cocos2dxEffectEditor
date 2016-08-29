//create by MatrixTan

uniform float u_time;
uniform sampler2D u_normal;
uniform vec2 u_maxuv;

varying vec2 v_texcoord;
vec4 base_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(void)
{
    vec2 clampuv = v_texcoord * u_maxuv;
    vec2 ripple_uv = mod(clampuv + vec2(0.0, u_time * 0.1), 1.0);
    
    vec3 normal_color = texture2D(u_normal, v_texcoord).xyz;
    vec3 normal_value = normalize(normal_color.xyz * 2.0 - 1.0);
    vec2 fromuv = normal_value.xy * 0.05;
    
    vec4 caustics_color = texture2D(CC_Texture0, ripple_uv + fromuv);
    gl_FragColor = caustics_color;
}