#include "ParticleSystem.h"
#include <sstream>
#include <iostream>

ParticleSystem::ParticleSystem(unsigned int maxParticles, const shared_ptr<Shader>& shader,
                               const shared_ptr<ComputeShader>& computeShader)
    : m_maxParticles(maxParticles), m_shader(shader),m_computeShader(computeShader) {
    InitBuffers();
}

ParticleSystem::~ParticleSystem() {
    glDeleteBuffers(1, &m_ssbo);
}

void ParticleSystem::InitBuffers() {
    // 初始化 SSBO
    m_computeShader->Bind();
    glGenBuffers(1, &m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    
    // 填充默认数据
    m_particles = vector<Particle>(m_maxParticles);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_maxParticles * sizeof(Particle), m_particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    // 初始化 VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_ssbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));
    glBindVertexArray(0);
}

void ParticleSystem::Update(float deltaTime, float currentTime) {
    m_computeShader->Bind();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    m_computeShader->SetUniform1f("time", currentTime);
    m_computeShader->SetUniform1i("numParticles", 1000);
    m_computeShader->Dispatch((m_maxParticles + groupSize - 1) / groupSize, 1, 1);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_maxParticles * sizeof(Particle), m_particles.data());

//    cout<<m_particles[128].position.x << " "<<m_particles[10].position.y << " "<<m_particles[10].position.z <<endl;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleSystem::Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, bool billboard) {
    m_shader->Bind();
    m_shader->SetUniformMat4f("view", viewMatrix);
    m_shader->SetUniformMat4f("projection", projectionMatrix);
    m_shader->SetUniform1i("billboard", billboard ? 1 : 0);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_particles.size());
    glBindVertexArray(0); // 解绑 VAO
}