#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;
out float visibility;

struct Camera {
	mat4 projection;
	mat4 view;
    vec3 position;
};

struct Sky {
	vec3 color;
	float density;
};

uniform Camera camera;
uniform Sky sky;
uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = camera.projection * camera.view * vec4(FragPos, 1.0);

	
	// Calculate fog visibility
	vec4 relativeToCameraPosition = camera.view * (model * vec4(aPos, 1.0));
	float distanceFromCamera = length(relativeToCameraPosition.xyz);
	visibility = exp(-pow((distanceFromCamera * sky.density), 1.2));
	visibility = clamp(visibility, 0.0, 1.0);
} 