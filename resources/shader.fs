#version 330 core
out vec4 frag_color;

in vec3 v_color;
in vec2 v_texcoord;

// do not worry about if you cannot find sampler uniform
// in main.cpp, i guess it relates to GL_TEXTURE0 by default
// so in main.cpp, glActiveTexture(GL_TEXTURE0) will help
uniform sampler2D a_texture1;

void main()
{
    // GLSL has built-in function called texture that takes 
    // its first arg as the texture sampler and 
    // its second arg as the corr textcoordinates
    // To get a little funky we can also mix the resulting texture color 
    // with the vertex colors. We simply multiply the resulting texture color 
    // with the vertex color in the fragment shader to mix both colors: 
    frag_color = texture(a_texture1, v_texcoord) * vec4(v_color, 1.0);
}