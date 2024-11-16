#shader vertex
#version 420 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

uniform vec2 offsets[100];
out vec3 fColor;

void main() {
	vec2 offset = offsets[gl_InstanceID];
	gl_Position = vec4(aPos + offset, 0, 1);
	fColor = aColor;
}


#shader fragment
#version 420 core
out vec4 FragColor;

in vec3 fColor;

void main()
{
	FragColor = vec4(fColor, 1.0);
}