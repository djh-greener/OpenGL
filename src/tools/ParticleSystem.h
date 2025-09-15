#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
using namespace std;

class VertexArray;
class StorageBuffer;
class Shader;
class ComputeShader;
class Texture;
class ParticleSystem {
public:
    //std140
    struct alignas(16) Particle {
        glm::vec3 position = {0,0,0};
        float _padding1;
        glm::vec4 color = {0,0,0,0};
        float size = 0.0f;
        float lifetime = 0.0f;
        float birthtime = 0.0f;
        float age = 0.0f;
    };

    ParticleSystem(unsigned int maxParticles, const shared_ptr<Shader>& shader,
                   const shared_ptr<ComputeShader>& computeShader);
    ~ParticleSystem();

    void Update(float deltaTime, float currentTime);
    void Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, bool billboard = true);

private:
    void InitBuffers();

    unsigned int m_maxParticles = 10000;
    std::vector<Particle> m_particles;
    unsigned int groupSize = 64;

    shared_ptr<VertexArray> m_vao;
    shared_ptr<StorageBuffer> m_ssbo;

    shared_ptr<Texture> m_texture;
    shared_ptr<Shader> m_shader;
    shared_ptr<ComputeShader> m_computeShader;
};