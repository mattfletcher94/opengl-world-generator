#version 330 core
out vec4 FragColor;
in float visibility;

struct Sky {
	vec3 color;
	float density;
};

uniform Sky sky;


void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // set all 4 vector values to 1.0
	FragColor = mix(vec4(sky.color.rgb, 1.0), vec4(1.0, 1.0, 1.0, 1.0), visibility);


}