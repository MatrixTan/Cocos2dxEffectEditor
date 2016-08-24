#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord;
varying vec4 v_fragmentcolor;

void main(void)
{
    gl_FragColor = texture2D(CC_Texture0, v_texcoord) * v_fragmentcolor;
}
