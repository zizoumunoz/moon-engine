#pragma once

#include <MoonEngine/Inc/MoonEngine.h>

class DoColorState : public MoonEngine::AppState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Terminate() override;
private:
	using Vertices = std::vector<MoonEngine::Graphics::VertexPC>;
	Vertices m_vertices;

	MoonEngine::Graphics::MeshBuffer m_meshBuffer;
	MoonEngine::Graphics::VertexShader m_vertexShader;
	MoonEngine::Graphics::PixelShader m_pixelShader;
	MoonEngine::Graphics::Camera m_camera;
	MoonEngine::Graphics::ConstantBuffer m_transformBuffer;
};
