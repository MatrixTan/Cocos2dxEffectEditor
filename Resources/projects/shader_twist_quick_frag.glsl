
uniform float u_time;
uniform float u_random_start;
varying vec2 v_texcoord;

void main(void)
{
    float time = u_time + u_random_start;
    vec2 uv = v_texcoord + vec2(cos(time * 43.0), sin(time * 28.0)) * 0.05;
    vec4 color = texture2D(CC_Texture0 , uv);
    float timeFactor = sin(u_time * 5.0) * 3.8 + 1.0;
    timeFactor = max(timeFactor, 1.0);
    gl_FragColor = vec4(color.xyz * timeFactor, color.a);
}