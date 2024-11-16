#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


uniform mat4 model;
uniform mat4 LightSpaceMatrix;

void main() {
	gl_Position = LightSpaceMatrix * model * vec4(aPos, 1);
}

#shader fragment
#version 420 core

void main() {
}