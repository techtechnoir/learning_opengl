#version 330 core
out vec4 frag_color;

in vec2 v_texcoord;

uniform sampler2D a_texture1;
uniform sampler2D a_texture2;
uniform float mix_amount;

void main()
{
    // linearly interpolate between both textures by mix_amount
    frag_color = mix(texture(a_texture1, v_texcoord), texture(a_texture2, vec2(v_texcoord.x, v_texcoord.y)), mix_amount);
}