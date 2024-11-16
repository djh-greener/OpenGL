#shader vertex
#version 420 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aOffset;

out vec3 fColor;
void main() {
	vec2 pos = aPos * (gl_InstanceID / 100.0);
	gl_Position = vec4(pos + aOffset, 0, 1);
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