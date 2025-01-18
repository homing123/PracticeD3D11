#pragma once

#include "ModelLoader.h"
#include "MeshData.h"

using namespace std;
using namespace DirectX;
using DirectX::SimpleMath::Matrix;

class MeshFactory
{
public:
	static MeshData CreateSquare(const float scale = 1.0f);
	static MeshData CreateBox(const float scale = 1.0f);
	static MeshData CreateSphere(const float radius, const int numSlices, const int numStacks);
	static vector<MeshData> ReadFromFile(std::string basePath, std::string filename);
};