uniform float u_time;
uniform float u_window_width; //窗口宽度，[0,1]范围内，材质宽度的比例。
uniform float u_speed;
uniform vec4 u_color;

varying vec2 v_texcoord;

void main(void)
{
    vec2 uv;
    uv.x = v_texcoord.x;
    float uvY = v_texcoord.y * u_window_width + u_speed * u_time - u_window_width;
    uv.y = mod(uvY, 1.0);
    vec4 color = texture2D(CC_Texture0, uv) * u_color;
    float topDiscard = step(0.0, uvY);
    gl_FragColor = color * topDiscard;
}
