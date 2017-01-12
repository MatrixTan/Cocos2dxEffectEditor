#ifdef GL_ES
precision highp float;
#endif

uniform float u_time;
varying vec2 v_texcoord;


void main(void)
{
    float factor1 = cos(v_texcoord.y * 3.0 + u_time * 6.0) * 0.1;
    
    vec2 uv = v_texcoord + vec2(factor1, 0.0) * (v_texcoord.y * v_texcoord.y);
    vec4 color = texture2D(CC_Texture0, uv);
    color.a = step(0.0, uv.x) * color.a;
    color.a = step(uv.x, 1.0) * color.a;
    color.a = step(0.0, uv.y) * color.a;
    color.a = step(uv.y, 1.0) * color.a;
    gl_FragColor = color * color.a;
}
