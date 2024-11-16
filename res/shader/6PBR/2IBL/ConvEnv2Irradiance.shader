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

const float PI = 3.14159265359;
uniform samplerCube EnvMap;

out vec4 FragColor;

void main() {
	vec3 N = normalize(LocalPos);
	vec3 Irradiance = vec3(0.0);
	vec3 Up = vec3(0.0, 1.0, 0.0);
	vec3 Right = normalize(cross(Up, N));
	Up = normalize(cross(N, Right));//N垂直UP,RIGHT的平面

	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	for (float phi = 0; phi < 2 * PI; phi += sampleDelta)
	{
		for (float theta = 0; theta < PI / 2; theta += sampleDelta)
		{
			vec3 Direction = vec3(0);
			Direction += N * cos(theta);
			Direction += Up * sin(theta) * sin(phi);
			Direction += Right * sin(theta) * cos(phi);
			Irradiance += texture(EnvMap, Direction).xyz * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	Irradiance = PI * Irradiance * (1 / float(nrSamples));//蒙特卡罗积分
	FragColor = vec4(Irradiance, 1);
}