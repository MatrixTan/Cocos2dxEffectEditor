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
    //float u_time = u_time * 0.1;
    float scalex = u_uv_rect.y - u_uv_rect.x;
    float scaley = u_uv_rect.w - u_uv_rect.z;
    //横向控制参数，越往下v_texcoord.y数值越大，控制参数越小。保证上方的摆动幅度大。
    float factor_x = cos((v_texcoord.y - u_uv_rect.z)/scaley * 1.57) * 1.5;
    //横向时间控制参数，线性减弱。
    float time_factor_x = max((1.0 - u_time)/1.0 , 0.0);
    vec2 uv;
    //横向余弦摆动
    uv.x = v_texcoord.x + cos((u_time + u_random_start)* 20.0) * 0.1 * factor_x * time_factor_x * scalex;
    
    //纵向时间控制，保证下蹲动作早完成0.5s
    float time_factor_y = max(time_factor_x - 0.5, 0.0);
    //纵向时间控制参数，时间的二次曲线与余弦曲线结合，由于要作为指数出现，要确保终点为1
    time_factor_y = pow(time_factor_y, 2.0) * cos(u_time * 10.0) * 5.0 + 1.0;
    
    uv.y = u_uv_rect.z + pow((v_texcoord.y - u_uv_rect.z)/scaley, time_factor_y)*scaley;
    
    //避免采样越界
    uv.x = clamp(uv.x, u_uv_rect.x, u_uv_rect.y);
    uv.y = clamp(uv.y, u_uv_rect.z, u_uv_rect.w);

    gl_FragColor = texture2D(CC_Texture0,uv);
    //gl_FragColor = vec4(v_texcoord.x, v_texcoord.y, 0.0, 1.0);
}
