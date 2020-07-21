//Vertex Shader
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int subtex;
uniform float tex_wid;
void main()
{
   //TexCoord = vec2(aTexCoord.x, aTexCoord.y);
   //TexCoord = aTexCoord;
   TexCoord = vec2((subtex*tex_wid)+aTexCoord.x*(tex_wid) , aTexCoord.y); 
   gl_Position = projection*view*model*vec4(aPos, 0.0f,  1.0);
};
