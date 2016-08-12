#ifdef GL_ES
precision highp float;
#endif

uniform float u_time;
varying vec2 v_texcoord;

void main(void)
{
    float time_factor = 1.0 - sin(min(u_time * 2.0, 1.5708));
    vec2 offset = v_texcoord - vec2(0.5, 0.5);
    vec2 uv = v_texcoord - offset * cos(u_time * 20.0) * 0.3 * time_factor;
    gl_FragColor = texture2D(CC_Texture0 , uv);
}