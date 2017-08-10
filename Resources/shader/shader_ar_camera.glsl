#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord;
uniform sampler2D u_textureUV;

mat4 ycbcr_rgb = mat4(
                          vec4(1.0000, 1.0000, 1.0000, 0.0000),
                          vec4(0.0000, -0.3441, 1.7720, 0.0000),
                          vec4(1.4020, -0.7141, 0.0000, 0.0000),
                          vec4(-0.7010, 0.5291, -0.8860, 1.0000));

void main(void)
{
    vec4 ycolor = texture2D(CC_Texture0, v_texcoord);
    vec4 uv_color = texture2D(u_textureUV, v_texcoord);
    vec4 ycbcr = vec4(ycolor.r, uv_color.r, uv_color.a, 1.0);
    gl_FragColor =  ycbcr_rgb * ycbcr;
}

