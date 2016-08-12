//create by MatrixTan

uniform float u_time;
uniform sampler2D u_normal;
uniform sampler2D u_star;

varying vec2 v_texcoord;
vec4 base_color = vec4(0.125, 0.373, 1.0, 1.0);

void main(void)
{
    vec2 ripple_uv = mod(v_texcoord + vec2(u_time * 0.1, 0.0), 1.0);
    vec3 ripple_color = texture2D(u_normal, ripple_uv).xyz;
    vec3 ripple_normal = normalize(ripple_color.xyz * 2.0 - 1.0);
    vec2 fromuv = ripple_normal.xy * 0.05;
    
    vec4 caustics_color = texture2D(CC_Texture0, fromuv + v_texcoord);
    vec4 star_color = texture2D(u_star, fromuv * 0.2 + v_texcoord);
    gl_FragColor = base_color + caustics_color * 0.8 + star_color * 0.3;
}