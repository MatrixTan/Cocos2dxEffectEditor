varying vec2 v_texcoord;

uniform float u_time;
uniform sampler2D u_erode_map;

void main(void)
{
    vec4 color = texture2D(CC_Texture0, v_texcoord);
    float erode = texture2D(u_erode_map, v_texcoord).r;
    erode = erode * u_time * 2.5;
    float alpha = min(1.0 - step(0.5, erode) * color.a * erode, color.a);
    gl_FragColor = color * alpha;
}
