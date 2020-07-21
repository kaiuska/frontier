//Fragment Shader
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex1;
uniform vec4 highlight;

void main()
{
    vec4 col = texture2D(tex1, TexCoord);
    if(col.a < 0.5 || col.rgb == vec3(0.0, 0.0, 0.0)){
    //if(col.a < 0.9){
        discard;
    }
    FragColor = col + highlight;
};


