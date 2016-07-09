#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord;
varying vec4 v_fragmentcolor;

void main(void)
{
    gl_FragColor = texture2D(CC_Texture0, v_texcoord);
    //gl_FragColor = vec4(v_texcoord.y, 0.0, 0.0, 1.0);
    //gl_FragColor = v_fragmentcolor;
}
