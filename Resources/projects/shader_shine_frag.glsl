#ifdef GL_ES
precision highp float;
#endif

uniform float u_time;
uniform sampler2D u_normalmap;

varying vec2 v_texcoord;

float diffuseFactor = 0.5;

void main(void)
{
    vec4 color = texture2D(CC_Texture0, v_texcoord);
    vec3 color2 = texture2D(u_normalmap, v_texcoord).xyz;

    vec3 lightDir = normalize(vec3((cos(u_time * 2.0) * 3.0), 0.0 ,1.0));
    float diffuse = clamp(dot(light_dir, color2), 0.0, 1.0);
    float specular = pow(diffuse, 5.0);

    gl_FragColor = vec4(color.xyz * 0.8 + color.xyz * diffuseFactor * specular, color.w);
}
