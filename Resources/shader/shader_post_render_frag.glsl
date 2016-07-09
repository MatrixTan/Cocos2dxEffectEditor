#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord;
varying vec4 v_fragmentcolor;
varying vec4 v_position;

uniform float time;
uniform vec2 center;
uniform vec2 winsize;

void main(void)
{
    vec4 color = texture2D(CC_Texture0, v_texcoord);
    vec2 offset = (v_position.xy - center) * winsize;
    color.r = cos(length(offset) * 0.02 - time);
    gl_FragColor = color;
    //gl_FragColor = vec4(v_texcoord.y, 0.0, 0.0, 1.0);
    //gl_FragColor = v_fragmentcolor;
}
