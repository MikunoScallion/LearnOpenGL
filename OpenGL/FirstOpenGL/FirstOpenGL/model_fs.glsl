#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflect1;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	vec3 reflectMap = texture(texture_reflect1, TexCoords).rgb;
	vec3 reflection = texture(skybox, R).rgb;
	vec3 sumReflect = mix(reflection, reflectMap, 0.2);

	vec3 diffuse = texture(texture_diffuse1, TexCoords).rgb;
	float diffuseDensity = 0.8;
	
	vec3 specular = texture(texture_specular1, TexCoords).rgb;
	float specularDensity = 0.5;

	vec3 result = diffuseDensity *  diffuse + specularDensity * specular * sumReflect;

	FragColor = vec4(result, 1.0);
}