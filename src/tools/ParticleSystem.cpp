#include "ParticleSystem.h"
#include "StorageBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "Path.h"
#include <iostream>

ParticleSystem::ParticleSystem(unsigned int maxParticles, const shared_ptr<Shader>& shader,
                               const shared_ptr<ComputeShader>& computeShader)
    : m_maxParticles(maxParticles), m_shader(shader),m_computeShader(computeShader) {
    InitBuffers();
}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::InitBuffers() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_particles = vector<Particle>(m_maxParticles);
    m_ssbo = make_shared<StorageBuffer>(m_particles.data(), m_maxParticles * sizeof(Particle));
    m_vao =  make_shared<VertexArray>();
    auto layout  = VertexBufferLayout();
    layout.Push<float>(3);
    layout.PushPadding<float>(1);
    layout.Push<float>(4);
    layout.Push<float>(1);
    layout.PushPadding<float>(3);
    m_vao->AddBuffer(*m_ssbo, layout);
    m_texture = make_shared<Texture>(GetResDir() + "res/image/particle/point.png");
}

void ParticleSystem::Update(float deltaTime, float currentTime) {
    m_computeShader->Bind();
    m_ssbo->Bind(0);
    m_computeShader->SetUniform1f("time", currentTime);
    m_computeShader->SetUniform1i("numParticles", 1000);
    m_computeShader->Dispatch((m_maxParticles + groupSize - 1) / groupSize, 1, 1);
    m_ssbo->Bind();
    m_ssbo->GetData(m_particles.data(), m_maxParticles * sizeof(Particle), 0);

    auto index = 127;
//    cout<<m_particles[index].position.x << " "<<m_particles[index].position.y << " "<<m_particles[index].position.z <<endl;
//    cout<<m_particles[index].size <<endl;

    m_ssbo->UnBind();
}

void ParticleSystem::Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, bool billboard) {

    m_shader->Bind();
    m_shader->SetUniformMat4f("view", viewMatrix);
    m_shader->SetUniformMat4f("projection", projectionMatrix);
    m_shader->SetUniform1i("billboard", billboard ? 1 : 0);

    m_texture->Bind(0);
    m_shader->SetUniform1i("tex", 0);


    m_vao->Bind();
    glDrawArrays(GL_POINTS, 0, m_particles.size());
    m_vao->UnBind();
}