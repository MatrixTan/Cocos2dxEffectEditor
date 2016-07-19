
uniform float u_time;
uniform float u_random_start;
varying vec2 v_texcoord;

void main(void)
{
    float time = u_time + u_random_start;
    vec2 uv = v_texcoord + vec2(cos(time * 33.0), sin(time * 22.0)) * 0.02;
    gl_FragColor = texture2D(CC_Texture0 , uv);
}