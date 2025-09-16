#include "DisAppearTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "Model.h"
#include "ParticleSystem.h"
#include "ComputeShader.h"
#include "FrameBuffer.h"

DisAppearTest::DisAppearTest()
{
    glEnable(GL_PROGRAM_POINT_SIZE);
	model = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj",true);
	Shaders["Nanosuit"] = make_shared<Shader>(GetResDir() + "res/shader/7Shaders/DisAppear/Nanosuit.glsl");
    Shaders["Particle"] = make_shared<Shader>( GetResDir() + "res/shader/7Shaders/DisAppear/ParticleRender.glsl");
    Shaders["Quad"] = make_shared<Shader>( GetResDir() + "res/shader/7Shaders/DisAppear/Quad.glsl");
    auto computeShader = make_shared<ComputeShader>(GetResDir() + "res/shader/7Shaders/DisAppear/ParticleCompute.glsl");
    FramebufferSpecification spec;
    spec.Width = SCR_WIDTH;
    spec.Height = SCR_HEIGHT;
    spec.Attachments = { AttachmentType::RGBA16F, AttachmentType::Depth };
    fbo = make_shared<FrameBuffer>(spec);
    particleSystem = make_shared<ParticleSystem>(1000, Shaders["Particle"], computeShader);

    float QuadVertices[] = {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
    {
        objects.push_back(make_shared<Object>());
        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(QuadVertices, sizeof(QuadVertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(2);
        objects[0]->layout->Push<float>(2);
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
    }
}

void DisAppearTest::OnRender()
{
    fbo->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glm::mat4 modelMat = glm::mat4(1);
	const float cycleDuration = 8.f;
	float time = fmod(glfwGetTime(), cycleDuration)/ cycleDuration;

	Shaders["Nanosuit"]->Bind();
	modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, glm::vec3(4, 1, 0));
	Shaders["Nanosuit"]->SetUniformMat4f("model", modelMat);
	Shaders["Nanosuit"]->SetUniformMat4f("view", m_View);
	Shaders["Nanosuit"]->SetUniformMat4f("projection", m_Proj);
	Shaders["Nanosuit"]->SetUniform3f("LightDir", glm::vec3(0, 0, -1));
	Shaders["Nanosuit"]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
	Shaders["Nanosuit"]->SetUniform3f("minVertex", modelMat * glm::vec4(model->minVertex,1.f));
	Shaders["Nanosuit"]->SetUniform3f("maxVertex", modelMat * glm::vec4(model->maxVertex,1.f));
	Shaders["Nanosuit"]->SetUniform1f("progress", time);
	model->DrawModel(Shaders["Nanosuit"]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    particleSystem->Render(m_View, m_Proj, false);
    glDisable(GL_BLEND);
    auto rgba = fbo->GetColorAttachmentRendererID(0);
    fbo->UnBind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    Texture::Bind(0,rgba);
    Shaders["Quad"]->Bind();
    Shaders["Quad"]->SetUniform1i("RGBA", 0);
    objects[0]->vao->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DisAppearTest::OnImGuiRender()
{
}

void DisAppearTest::OnUpdate(float deltaTime) {
    particleSystem->Update(deltaTime, static_cast<float>(glfwGetTime()));
}
