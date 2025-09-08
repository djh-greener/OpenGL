#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "Shader.h"
#include "ComputeShader.h"
class VertexArray;
class VertexBuffer;
class ParticleSystem {
public:
    //std140
    struct alignas(16) Particle {
        glm::vec3 position = {0,0,0};
        float _padding1; // Ìî³ä vec3 µ½ 16 ×Ö½Ú
        glm::vec4 color = {0,0,0,0};
        float size = 0;
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

    GLuint m_ssbo;
    GLuint m_vao;

    shared_ptr<Shader> m_shader;
    shared_ptr<ComputeShader> m_computeShader;
};