//create by MatrixTan

#ifdef GL_ES
precision highp float;
#endif

uniform float u_time;
uniform float u_random_start;
uniform vec4 u_uv_rect;
varying vec2 v_texcoord;

void main(void)
{
    float scalex = u_uv_rect.y - u_uv_rect.x;
    float scaley = u_uv_rect.w - u_uv_rect.z;
    float factor = cos((v_texcoord.y - u_uv_rect.z)/scaley * 1.57) * 1.5;
    float time_factor_x = max((1.0 - u_time)/1.0 , 0.0);
    vec2 uv;
    uv.x = v_texcoord.x + cos((u_time + u_random_start)* 20.0) * 0.1 * factor * time_factor_x * scalex;
    float time_factor_y = max(time_factor_x - 0.5, 0.0);
    time_factor_y = pow(time_factor_y, 2.0) * cos(u_time * 10.0) * 5.0 + 1.0;
    uv.y = u_uv_rect.z + pow(v_texcoord.y - u_uv_rect.z, time_factor_y);
    //uv.y = v_texcoord.y;
    gl_FragColor = texture2D(CC_Texture0,uv);
}