//create by MatrixTan

varying vec2 v_texcoord;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float u_contrast = 0.0;
uniform float u_hue;
uniform float u_saturation;
uniform float u_value;

void main(void)
{
    vec4 color = texture2D(CC_Texture0,v_texcoord);
    vec3 hsv;
    hsv.xyz = rgb2hsv(color.rgb);
    hsv.x += u_hue;
    hsv.x = mod(hsv.x, 1.0);
    hsv.y *= u_saturation;
    hsv.z *= u_value;
    vec3 f_color = hsv2rgb(hsv);
    f_color = ((f_color - 0.5) * max(u_contrast+1.0, 0.0)) + 0.5;
    gl_FragColor = vec4(f_color, color.a);
}