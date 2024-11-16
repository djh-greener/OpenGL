#shader vertex
#version 420 core
layout(location = 0)in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 LocalPos;
void main()
{
	LocalPos = aPos;
	gl_Position = projection * view * vec4(aPos, 1);
}

#shader fragment
#version 420 core
in vec3 LocalPos;

uniform sampler2D equirectangularMap;
out vec4 FragColor;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
	//三维直角坐标--->极坐标
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));//u=[-pi,pi]  v=[-pi/2,pi/2]
	uv *= invAtan; uv += 0.5;//映射到[0,1]
	return uv;
}
void main()
{
	vec2 uv = SampleSphericalMap(normalize(LocalPos)); // make sure to normalize localPos
	vec3 color = texture(equirectangularMap, uv).rgb;

	FragColor = vec4(color, 1.0);
}
