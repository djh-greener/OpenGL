#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


uniform mat4 model;

void main() {
	gl_Position = model * vec4(aPos, 1);
}

#shader geometry
#version 420 core
layout(triangles)in;
layout(triangle_strip, max_vertices = 18)out;

uniform mat4 ShadowMatrices[6];

out vec4 FragPos;

void main() {
	for (int face = 0; face < 6; face++) {
		gl_Layer = face;//帧缓冲绑定的6个纹理层
		for (int i = 0; i < 3; i++) {
			FragPos = gl_in[i].gl_Position;
			gl_Position = ShadowMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}




#shader fragment
#version 420 core
in vec4 FragPos;

uniform vec3 LightPos;
uniform float FarPlane;
void main() {
	gl_FragDepth = length(FragPos.xyz - LightPos)/FarPlane;
}