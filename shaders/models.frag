#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in float visibility;

struct Light {
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct Camera {
	mat4 projection;
	mat4 view;
    vec3 position;
};

struct Sky {
	vec3 color;
	float density;
};

  
// Uniforms
uniform Camera camera;
uniform Light light;
uniform Sky sky;
uniform sampler2D texture0;



// Main
void main()
{

	// Get texture color
	vec4 finalTextureColor = texture(texture0, TexCoords);

	// Caluclate ambient light
	vec3 ambient = light.ambient * finalTextureColor.rgb;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * finalTextureColor.rgb;  
    
    // specular
    vec3 viewDir = normalize(camera.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
    vec3 specular = light.specular * spec * finalTextureColor.rgb;  
    
    // calculate attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	// Add attenuation to lighting
    ambient *= attenuation;  
    diffuse *= attenuation;
    specular *= attenuation;   
        
	// Add lighting to output color
    vec3 result = ambient + diffuse + specular;

	// Also merge output color with fog visibility
	FragColor = mix(vec4(sky.color.rgb, 1.0), vec4(result, 1.0), visibility);

}
