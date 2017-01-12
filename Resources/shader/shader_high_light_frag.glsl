//create by MatrixTan
#ifdef GL_ES
precision highp float;
#endif
uniform float u_time;
uniform float u_speed;

varying vec2 v_texcoord;

void main(void)
{
    vec4 star_color = texture2D(CC_Texture0, v_texcoord);
    star_color = star_color * (1.5 + sin(u_time * u_speed) * 0.5);
    gl_FragColor = star_color;
}