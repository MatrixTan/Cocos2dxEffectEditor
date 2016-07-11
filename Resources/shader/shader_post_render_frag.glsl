#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord;
varying vec4 v_fragmentcolor;
varying vec4 v_position;

uniform float time;
uniform vec2 center;
uniform vec2 winsize;

float radius = 400.0; //wave radius

void main(void)
{
    float t = time;
    //t = time * step(time, 0.6);
    vec4 color = texture2D(CC_Texture0, v_texcoord);
    vec2 offset = (v_position.xy - center) * winsize;
    float offsetLength = length(offset);
    float lengthFactor = max((radius - offsetLength)/radius, 0.0)*offsetLength/radius;
    float timeLine = length(offset) * 0.01 - t * 8.0;
    float uvLength = max(cos(timeLine) * 0.1, 0.0);
    vec2 uvOffset = normalize(offset) * uvLength * lengthFactor * min(t * 10.0, 1.0);
    color = texture2D(CC_Texture0, v_texcoord - uvOffset);
    gl_FragColor = color;
}
