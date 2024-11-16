#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1);

}

#shader fragment
#version 420 core

out vec4 FragColor;

void main() {
	FragColor = vec4(0, 1, 0, 1);
}