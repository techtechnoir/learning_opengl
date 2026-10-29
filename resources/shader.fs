#version 330 core
out vec4 frag_color;

in vec3 v_color;
in vec2 v_texcoord;

uniform sampler2D a_texture1;
uniform sampler2D a_texture2;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface) 
    frag_color = mix(texture(a_texture1, v_texcoord), texture(a_texture2, v_texcoord), 0.2);
}