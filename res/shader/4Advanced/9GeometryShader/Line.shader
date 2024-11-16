#shader vertex
#version 420 core
layout(location = 0) in vec2 aPos;

void main()
{
	gl_Position = vec4(aPos.x,aPos.y,0, 1.0);
};



#shader geometry
#version 420 core
layout(points) in;
layout(line_strip, max_vertices = 2) out;
void main() {
	gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}

#shader fragment
#version 420 core

out vec4 FragColor;
void main()
{
	FragColor = vec4(0, 1, 0, 1);
}