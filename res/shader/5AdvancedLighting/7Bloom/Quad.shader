#shader vertex
#version 420 core
layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(Pos,0, 1.0);
};

#shader fragment
#version 420 core
in vec2 TexCoord;
uniform sampler2D Diffuse;
out vec4 FragColor;
void main()
{

	FragColor = vec4(texture(Diffuse, TexCoord).rgb,1);
}
