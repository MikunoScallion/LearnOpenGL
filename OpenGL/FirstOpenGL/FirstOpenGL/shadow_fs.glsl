#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;

void main()
{   
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // 从贴图[0,1]转换为法线[-1,1]
	normal = normalize(normal * 2.0 - 1.0);

    vec3 lightColor = vec3(0.8);
    vec3 ambient = 0.3 * color;
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.8;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
	float bias = max(0.09 * (1.0 - dot(normal, lightDir)), 0.01);
    vec3 lighting = (ambient + diffuse + specular) * color;    

    FragColor = vec4(lighting, 1.0);
}