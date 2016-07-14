attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

varying vec2 v_texcoord;
varying vec4 v_fragmentcolor;

void main()
{
    gl_Position = CC_PMatrix * a_position;
    v_fragmentcolor = a_color;
    v_texcoord = a_texCoord;
}