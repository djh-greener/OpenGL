#shader vertex
#version 330 core
layout(location = 0) in vec3 Pos;
void main()
{
	gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);
};

#shader fragment
#version 330 core

out vec4 FragColor;
void main()
{
	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
};