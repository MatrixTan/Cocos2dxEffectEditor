#ifdef GL_ES
precision highp float;
#endif

uniform float u_time;
varying vec2 v_texcoord;

vec2 point1 = vec2(1.1, 1.2);
vec2 point2 = vec2(-1.0, 1.3);
vec2 point3 = vec2(0.5, 0.4);

void main(void)
{
    vec2 offset1 = v_texcoord - point1;
    float distance1 = length(offset1);
    float factor1 = cos(distance1 * 12.0 + u_time * 17.0) * 0.002;
    offset1 = normalize(offset1) * factor1;
    
    vec2 offset2 = v_texcoord - point2;
    float distance2 = length(offset2);
    float factor2 = cos(distance2 * 2.0 + u_time * 23.0) * 0.009;
    offset2 = normalize(offset2) * factor2;
    
    
    vec2 offset3 = v_texcoord - point3;
    float distance3 = length(offset3);
    float factor3 = sin(distance3 * 7.0 + u_time * 17.0) * 0.007;
    offset3 = normalize(offset3) * factor3;
 
    vec2 offset = offset1 + offset2 + offset3;
    
    vec2 uv = v_texcoord + offset;
    vec4 color = texture2D(CC_Texture0, uv);
    color.a = step(0.0, uv.x) * color.a;
    color.a = step(uv.x, 1.0) * color.a;
    color.a = step(0.0, uv.y) * color.a;
    color.a = step(uv.y, 1.0) * color.a;
    gl_FragColor = color * color.a;
}
