#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;


out vec3 TexCoords;

void main()
{
	gl_Position = projection*view*vec4(aPos, 1.0);
	TexCoords = aPos;
}

#shader fragment
#version 330 core

in vec3 TexCoords;
uniform sampler2D screenTexture;

vec3 envSample(vec3 dir) {
	float theta = acos(dir.y);
	float phi = atan(dir.z, dir.x);
	vec2 uv = vec2(phi / (2 * 3.1415926) + 0.5, theta / 3.1415926);
	return texture2D(screenTexture, uv,0).rgb;
}

out vec4 FragColor;
void main()
{
	FragColor = vec4(envSample(TexCoords), 1);
}