#shader vertex
#version 330 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Color;


out vec3 PosTmp;
void main()
{
	gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);
	PosTmp = Pos;
};

#shader fragment
#version 330 core
in vec3 PosTmp;

out vec4 FragColor;
void main()
{
	FragColor = vec4(PosTmp, 1.0);
};