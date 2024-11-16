#shader vertex
#version 420 core
layout(location = 0)in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;

out vec3 LocalPos;
void main() {
	LocalPos = aPos;
	vec4 Pos = projection * mat4(mat3(view)) * vec4(aPos, 1);
	gl_Position = Pos.xyww;
}

#shader fragment
#version 420 core
in vec3 LocalPos;

uniform samplerCube CubeMap;

out vec4 FragColor;

void main() {
	vec3 Color = texture(CubeMap, normalize(LocalPos)).xyz;
	Color = Color / (Color + vec3(1));
	Color = pow(Color, vec3(1 / 2.2));
	FragColor = vec4(Color, 1);
}