#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 FragColor;
void main()
{
	float depth = gl_FragCoord.z;
	depth = depth * 2 - 1;
	float near = 0.1;
	float far = 100;
	float linearDepth = (2.0 * near ) / (far + near - depth * (far - near));
	FragColor = vec4(vec3(linearDepth), 1.0);
}