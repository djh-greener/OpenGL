#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(std140, binding = 0)uniform Matrices
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
};

#shader fragment
#version 420 core

uniform vec3 Color;

out vec4 FragColor;
void main()
{

	FragColor = vec4(Color,1);
}