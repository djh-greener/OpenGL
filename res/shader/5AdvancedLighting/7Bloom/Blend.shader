#shader vertex
#version 420 core
layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(Pos, 0, 1.0);
};

#shader fragment
#version 420 core
in vec2 TexCoord;
uniform sampler2D Scene;
uniform sampler2D BloomBlur;
uniform float exposure;


out vec4 FragColor;
void main()
{
	const float gamma = 2.2;
	vec3 HdrColor = texture(Scene, TexCoord).rgb;
	vec3 BloomColor = texture(BloomBlur, TexCoord).rgb;
	HdrColor += BloomColor; // additive blending
	// tone mapping
	vec3 result = vec3(1.0) - exp(-HdrColor * exposure);
	// also gamma correct while we're at it       
	result = pow(result, vec3(1.0 / gamma));
	FragColor = vec4(result, 1);
}
