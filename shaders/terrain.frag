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

struct BlendMap {
	sampler2D backgroundTexture;
	sampler2D redTexture;
	sampler2D greenTexture;
	sampler2D blueTexture;
	sampler2D blendMapTexture;
};

  
// Uniforms
uniform Camera camera;
uniform Light light;
uniform Sky sky;
uniform BlendMap blendMap;


// Main
void main()
{

	// Blend map calculation
	// Use the blend map pixel color to determine
	// how much of each textures color makes up the final terrain pixel color
	vec4 blendMapColor			= texture(blendMap.blendMapTexture, TexCoords);
	float backTextureAmount		= 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords			= TexCoords * 5.0f;
	vec4 backgroundTextureColor = texture(blendMap.backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 redTextureColor		= texture(blendMap.redTexture, tiledCoords) * blendMapColor.r;
	vec4 greenTextureColor		= texture(blendMap.greenTexture, tiledCoords) * blendMapColor.g;
	vec4 blueTextureColor		= texture(blendMap.blueTexture, tiledCoords) * blendMapColor.b;
	vec4 finalTextureColor		= backgroundTextureColor + redTextureColor + greenTextureColor + blueTextureColor;

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
    
	// spotlight (soft edges)
    /*float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;*/

    // calculate attenuation
    float distance = length(light.position - FragPos);
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