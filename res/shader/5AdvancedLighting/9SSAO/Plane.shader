#shader vertex
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}vs_out;

void main()
{
	//йсм╪©у╪Д
	vec4 ViewPos = view * model * vec4(aPos, 1.0);
	gl_Position = projection * ViewPos;

	vs_out.FragPos = vec3(ViewPos);
	vs_out.TexCoord = aTexCoord;
	vs_out.Normal = transpose(inverse(mat3(view * model))) * aNormal;
};

#shader fragment
#version 420 core
in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}fs_in;
layout(location = 0)out vec3 GPosition;
layout(location = 1)out vec3 GNormal;
layout(location = 2)out vec4 GColorSpec;

void main()
{
	GPosition.rgb = fs_in.FragPos;

	GNormal = normalize(fs_in.Normal);

	GColorSpec.rgb = vec3(0.5);
	GColorSpec.a = 0.5;
}