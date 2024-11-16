#shader vertex
#version 330 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Color;
uniform float offset;

out vec3 VertColor;
void main()
{
	gl_Position = vec4(Pos.x+offset, Pos.y, Pos.z, 1.0);
	VertColor = Color;
};

#shader fragment
#version 330 core
in vec3 VertColor;

out vec4 FragColor;
void main()
{
	FragColor = vec4(VertColor, 1.0);
};