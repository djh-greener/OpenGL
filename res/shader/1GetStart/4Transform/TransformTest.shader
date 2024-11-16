#shader vertex
#version 330 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 Tex;

uniform mat4 transform;

out vec2 TexCoord;
void main()
{
	gl_Position = transform*vec4(Pos, 1.0);
	TexCoord = Tex;
};

#shader fragment
#version 330 core
in vec2 TexCoord;
uniform sampler2D Texture0;//应绑定到纹理单元GL_TEXTURE0
uniform sampler2D Texture1;//应绑定到纹理单元GL_TEXTURE1
out vec4 FragColor;
void main()
{

	FragColor = mix(texture(Texture0, TexCoord), texture(Texture1, TexCoord),0.2);
};