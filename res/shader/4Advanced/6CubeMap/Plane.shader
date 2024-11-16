#shader vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
	vec4 pos = vec4(aPos.x, aPos.y, 0.0, 1.0);;
	gl_Position = projection * view * model * pos;
	TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;
out vec4 FragColor;
void main()
{
	FragColor = texture(screenTexture, TexCoords);
}