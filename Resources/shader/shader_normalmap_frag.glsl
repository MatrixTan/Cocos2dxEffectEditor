#ifdef GL_ES
precision highp float;
#endif

//uniform float u_time;
uniform sampler2D u_normalmap;
uniform vec3 u_light_dir;
uniform vec3 u_view_dir;

varying vec2 v_texcoord;

void main(void)
{
    vec3 light_dir = normalize(u_light_dir);
    vec3 high_light = normalize(u_light_dir + u_view_dir);
    vec4 color = texture2D(CC_Texture0, v_texcoord);
    vec3 normal = normalize(texture2D(u_normalmap, v_texcoord).xyz);
    float diffuse = max(0.0, dot(normal, light_dir));
    float normal_high_light = max(0.0, dot(normal, high_light));
    float specular = pow(normal_high_light, 128.0 * 1.2) * 0.4;
    vec4 out_color;
    out_color.rgb = color.rgb * diffuse + vec3(1.0, 1.0, 1.0) * specular;
    out_color.a = color.a;
    gl_FragColor = out_color;
}
