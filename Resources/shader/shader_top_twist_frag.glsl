//create by MatrixTan

uniform float u_time;
uniform vec4 u_uv_rect;

varying vec2 v_texcoord;

void main(void)
{
    float scalex = u_uv_rect.y - u_uv_rect.x;
    float scaley = u_uv_rect.w - u_uv_rect.z;
    float factor_x = cos((v_texcoord.y - u_uv_rect.z)/scaley * 1.57) * 2.5;
    float time_factor_x = max((1.0 - u_time)/0.6 , 0.0);
    vec2 uv;
    uv.x = v_texcoord.x + cos(u_time * 20.0) * 0.1 * factor_x * time_factor_x * scalex;
    uv.x = min(uv.x, u_uv_rect.y);
    uv.x = max(uv.x, u_uv_rect.x);
    uv.y = v_texcoord.y;
    gl_FragColor = texture2D(CC_Texture0,uv);
}