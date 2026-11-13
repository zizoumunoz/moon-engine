#pragma once

#include "MeshTypes.h"

namespace MoonEngine::Graphics
{
	class MeshBuilder
	{
	public:
		// cube
		static MeshPC CreateCubePC(float size, const Color& color);
		static MeshPC CreateCubePC(float size);

		// rectangle
		static MeshPC CreateRectanglePC(float width, float height, float depth);

		// plane
		static MeshPC CreatePlanePC(int numRows, int numColumns, float spacing, bool horizontal = true);

		// cylinder
		static MeshPC CreateCylinderPC(int slices, int rings);

		// sphere
		static MeshPC CreateSpherePC(int slices, int rings, float radius);

	};
}