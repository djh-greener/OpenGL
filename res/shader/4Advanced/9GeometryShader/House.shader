#shader vertex
#version 420 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
out VS_OUT{
	vec3 color;
} vs_out;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);
	vs_out.color = aColor;
};



#shader geometry
#version 420 core
layout(points) in;
layout(triangle_strip, max_vertices = 5) out;
in VS_OUT{
	vec3 color;
} gs_in[];

out vec3 fColor;
void build_house(vec4 position) {
	fColor = gs_in[0].color; // gs_in[0] 因为只有一个输入顶点
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);    // 2:右下
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);    // 3:左上
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);    // 4:右上
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);    // 5:顶部
	fColor = vec3(1,1,1); 
	EmitVertex();
}
void main() {
	build_house(gl_in[0].gl_Position);
}

#shader fragment
#version 420 core
in vec3 fColor;
out vec4 FragColor;
void main()
{
	FragColor = vec4(fColor, 1);
}