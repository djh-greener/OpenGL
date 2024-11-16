#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	 vec2 TexCoord;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.TexCoord = aTexCoord;
};

#shader geometry
#version 420 core
layout(triangles)in;
layout(triangle_strip,max_vertices =3)out;

in VS_OUT{
	 vec2 TexCoord;
}gs_in[];

uniform float time;

out vec2 TexCoord;

vec3 GetTriangleNormal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b= vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}
vec4 Explode(vec4 position,vec3 normal) {
	float magnitude = 2.0;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return position + vec4(direction, 0.0);
}
void main() {
	vec3 normal = GetTriangleNormal();

	gl_Position = Explode(gl_in[0].gl_Position,normal);
	TexCoord = gs_in[0].TexCoord;
	EmitVertex();

	gl_Position = Explode(gl_in[1].gl_Position,normal);
	TexCoord = gs_in[1].TexCoord;
	EmitVertex();

	gl_Position = Explode(gl_in[2].gl_Position,normal);
	TexCoord = gs_in[2].TexCoord;
	EmitVertex();
}

#shader fragment
#version 420 core
in vec2 TexCoord;
struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
};
uniform Material material;
out vec4 FragColor;
void main()
{
	//  Ù–‘

	vec3 result = vec3(texture(material.diffuse1, TexCoord));
	FragColor = vec4(result, 1.0);
}