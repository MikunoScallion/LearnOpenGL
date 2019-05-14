#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float outPercent;

void main()
{
    FragColor = mix(texture(texture1, vec2(1.0 - TexCoord.x, TexCoord.y)), texture(texture2, TexCoord), outPercent);
}