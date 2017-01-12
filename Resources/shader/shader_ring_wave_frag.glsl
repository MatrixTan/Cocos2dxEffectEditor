#ifdef GL_ES
precision highp float;
#endif

uniform float u_time;
uniform sampler2D u_normal_map;

varying vec2 v_texcoord;
float speed = 0.3;

void main(void)
{
  
    vec2 offset = texture2D(u_normal_map,  vec2(v_texcoord.x, v_texcoord.y * 0.5+ u_time * speed)).rg - vec2(0.5, 0.5);
    offset.x = 0.0;
    vec4 color = texture2D(CC_Texture0, v_texcoord + offset * 0.2);
    
    gl_FragColor = color;
}
