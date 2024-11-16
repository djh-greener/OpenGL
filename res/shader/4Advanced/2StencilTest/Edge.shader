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
	gl_Position = projection * view * model * vec4(aPos+0.02*aNormal, 1.0);
};

#shader fragment
#version 330 core

uniform vec3 EdgeColor;

out vec4 FragColor;
void main()
{
	FragColor = vec4(EdgeColor, 1.0);
}