#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;


void main()
{ 
	
	// Merge screen and bloom textures
    vec3 screenColor = texture(screenTexture, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomTexture, TexCoords).rgb;      
	vec3 result = screenColor + bloomColor;

	// Adding gamme and exposure
	const float gamma = 2.2;
	const float exposure = 1.5;

	result = vec3(1.0) - exp(-result * exposure);
	result = pow(result, vec3(1.0 / gamma));

	// Output frag color
	FragColor = vec4(result, 1.0);

}