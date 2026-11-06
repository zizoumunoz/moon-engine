#include "DoColorState.h"


using namespace MoonEngine;
using namespace MoonEngine::Graphics;
using namespace MoonEngine::Input;
void DoColorState::Initialize()
{
	// set up camera
	m_camera.SetPosition({ 0.0f, 1.0f, -3.0f });
	m_camera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	// create a cube
	//have 6 sides
	// each side is 2 triangles
	// each triangle is 3 vertices
	// vertcie must be in clockwise order
	
	// front 
	m_vertices.push_back({ { -1.f, 0.f, -1.f }, Colors::Red });
	m_vertices.push_back({ { 0.f, 1.f, -1 }, Colors::Red });
	m_vertices.push_back({ { 1, 0, 0 }, Colors::Red });

	m_vertices.push_back({ { -1, -1, -1 }, Colors::Blue });
	m_vertices.push_back({ { -1, 1, -1 }, Colors::Blue });
	m_vertices.push_back({ { 1, -1, -1 }, Colors::Blue });

	m_vertices.push_back({ { 1, -1, -1 }, Colors::Green });
	m_vertices.push_back({ { 1, 1, 1 }, Colors::Green });
	m_vertices.push_back({ { 1, -1, 1 }, Colors::Green });

	m_vertices.push_back({ { 1, -1, -1 }, Colors::Purple });
	m_vertices.push_back({ { 1, 1, 1 }, Colors::Purple });
	m_vertices.push_back({ { 1, -1, 1 }, Colors::Purple });



	// create a shape

	m_meshBuffer.Initialize(m_vertices.data(), sizeof(VertexPC), m_vertices.size());
	std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTransform.fx";
	m_vertexShader.Initialize<VertexPC>(shaderPath);
	m_pixelShader.Initialize(shaderPath);

	m_transformBuffer.Initialize(sizeof(Math::Matrix4));
}

void DoColorState::Update(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT)) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;
	if (input->IsKeyDown(KeyCode::W))
	{
		m_camera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		m_camera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		m_camera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		m_camera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		m_camera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		m_camera.Rise(-moveSpeed * deltaTime);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		m_camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		m_camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void DoColorState::Render()
{
	m_vertexShader.Bind();
	m_pixelShader.Bind();
	m_transformBuffer.BindVS(0);

	// update buffer data
	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = m_camera.GetViewMatrix();
	Math::Matrix4 matProj = m_camera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;
	Math::Matrix4 wvp = Math::Transpose(matFinal);
	m_transformBuffer.Update(&wvp);

	m_meshBuffer.Render();
}

void DoColorState::Terminate()
{
	m_vertices.clear();
	m_transformBuffer.Terminate();
	m_pixelShader.Terminate();
	m_vertexShader.Terminate();
	m_meshBuffer.Terminate();
}