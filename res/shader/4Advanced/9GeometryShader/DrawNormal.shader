#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;

out VS_OUT{
	vec3 Normal;
}vs_out;

void main()
{
	gl_Position = view * model * vec4(aPos, 1.0);
	vs_out.Normal = normalize(mat3(transpose(inverse(view * model)))*aNormal);
};




#shader geometry
#version 420 core
layout(triangles)in;
layout(line_strip, max_vertices = 6)out;
const float MAGNITUDE = 0.4;
uniform mat4 projection;

in VS_OUT{
	vec3 Normal;
}gs_in[];
void DrawLine(int index) {
	gl_Position = projection*gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = projection*(gl_in[index].gl_Position +vec4(gs_in[index].Normal,0)*MAGNITUDE);
	EmitVertex();

	EndPrimitive();
}
void main() {
	DrawLine(0);
	DrawLine(1);
	DrawLine(2);
}

#shader fragment
#version 420 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}