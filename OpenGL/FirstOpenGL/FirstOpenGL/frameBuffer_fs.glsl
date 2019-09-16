#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

vec3 ReinhardToneMapping(vec3 color, float adapted_lum) 
{
    const float MIDDLE_GREY = 1;
    color *= MIDDLE_GREY / adapted_lum;
    return color / (1.0f + color);
}

vec3 CEToneMapping(vec3 color, float adapted_lum) 
{
    return 1 - exp(-adapted_lum * color);
}

vec3 F(vec3 x)
{
	const float A = 0.22f;
	const float B = 0.30f;
	const float C = 0.10f;
	const float D = 0.20f;
	const float E = 0.01f;
	const float F = 0.30f;
 
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

vec3 Uncharted2ToneMapping(vec3 color, float adapted_lum)
{
	const vec3 WHITE = vec3(11.2f);
	return F(1.6f * adapted_lum * color) / F(WHITE);
}

vec3 ACESToneMapping(vec3 color, float adapted_lum)
{
	const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;

	color *= adapted_lum;
	return (color * (A * color + B)) / (color * (C * color + D) + E);
}

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if(hdr)
    {
        vec3 result = ACESToneMapping(hdrColor, exposure);
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}