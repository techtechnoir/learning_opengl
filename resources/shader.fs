#version 330 core
out vec4 frag_color;

uniform float our_color;

void main()
{
    frag_color = vec4(0.0, our_color, 0.0, 1.0);
}