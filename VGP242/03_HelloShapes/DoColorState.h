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
	struct Vertex
	{
		MoonEngine::Math::Vector3 position = MoonEngine::Math::Vector3::Zero;
		MoonEngine::Color color = MoonEngine::Colors::White;

	};

	using Vertices = std::vector<Vertex>;
	Vertices m_vertices;

	// our shape
	ID3D11Buffer* m_vertexBuffer = nullptr;
	// bind to the VS function
	ID3D11VertexShader* m_vertexShader = nullptr;
	// tell the vs configuration of the vertex
	ID3D11InputLayout* m_inputLayout = nullptr;
	// bind to the PS functdion
	ID3D11PixelShader* m_pixelShader = nullptr;
};
