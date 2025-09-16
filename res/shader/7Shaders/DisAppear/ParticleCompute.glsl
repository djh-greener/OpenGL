#version 430 core
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;
struct Particle {
    vec3 position;
    float padding;
    float size;
    float lifetime;
    float birthtime;
    float age;
};

layout(std140, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

uniform float time;

// 生成随机数的函数
float rand(uint seed) {
    return fract(sin(seed * 12.9898) * 43758.5453);
}

void main() {
    uint idx = gl_GlobalInvocationID.x;

    // 使用全局索引和时间作为随机种子
    uint seed = idx + uint(time * 1000.0);

    // 如果粒子年龄超过生命周期，重新生成粒子
    if (particles[idx].age >= particles[idx].lifetime || particles[idx].birthtime == 0.0) {
        // 生成[-2, 2]范围内的随机位置
        particles[idx].position = vec3(
        mix(-2.0, 2.0, rand(seed)),
        mix(-2.0, 2.0, rand(seed + 1u)),
        mix(-2.0, 2.0, rand(seed + 2u))
        );

        particles[idx].size = 0.1;
        particles[idx].lifetime = 1.0f;
        particles[idx].birthtime = time;
        particles[idx].age = 0.0f;
    } else {
        particles[idx].age = time -  particles[idx].birthtime;
    }
}
