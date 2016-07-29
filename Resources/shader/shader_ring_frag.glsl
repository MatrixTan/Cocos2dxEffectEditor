#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord;
//放大倍数
uniform float u_scale;
//径向缩放半径阈值，［0，1］范围内，是缩放区域直径与图片内切圆的比例。大于阈值部分的圆环将保留宽度。
uniform float u_threshold_ring;
void main(void)
{
    float halfR = u_threshold_ring * 0.5;
    vec2 offset = v_texcoord - vec2(0.5, 0.5);
    //计算缩放后，阈值UV半径
    float ringDistance = 0.5 - (0.5 - halfR) / u_scale;
    //本像素距中心的UV半径
    float dist = length(offset);
    //本像素的方向向量
    vec2 direction = normalize(offset);
    //阈值圆外部的UV坐标
    vec2 uvOutRing = (((dist - ringDistance) * u_scale + halfR) * direction + vec2(0.5, 0.5));
    vec2 uv = step(ringDistance, dist) * uvOutRing;
    uv += step(dist, ringDistance) * vec2(0.5, 0.5);
    vec4 color = texture2D(CC_Texture0, uv);
    gl_FragColor = color;
}
