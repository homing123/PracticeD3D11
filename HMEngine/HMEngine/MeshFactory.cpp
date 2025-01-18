#include "MeshFactory.h"

MeshData MeshFactory::CreateSquare(const float scale)
{
	vector<Vertex> v_vert = {
		//-z
		Vertex{Vector3(-0.5f, -0.5f, 0.f) * scale, Vector3(0,0,1), Vector2(0.f, 0.f)},
		Vertex{Vector3(-0.5f, 0.5f, 0.f) * scale,  Vector3(0,0,1), Vector2(0.f, 1.f)},
		Vertex{Vector3(0.5f, 0.5f, 0.f) * scale,   Vector3(0,0,1), Vector2(1.f, 1.f)},
		Vertex{Vector3(0.5f, -0.5f, 0.f) * scale,  Vector3(0,0,1), Vector2(1.f, 0.f)}
	};
	vector<uint32_t> v_index = { 0,1,2,0,2,3 };
	return MeshData(v_vert, v_index);
}

MeshData MeshFactory::CreateBox(const float scale)
{
	vector<Vertex> v_vert =
	{
		//+y
		Vertex{Vector3(-0.5f, 0.5f, -0.5f) * scale, Vector3(0, 1, 0), Vector2(0.f, 1.f)},
		Vertex{Vector3(-0.5f, 0.5f, 0.5f) * scale, Vector3(0, 1, 0), Vector2(0.f, 0.f)},
		Vertex{Vector3(0.5f, 0.5f, 0.5f) * scale, Vector3(0, 1, 0), Vector2(1.f, 0.f)},
		Vertex{Vector3(0.5f, 0.5f, -0.5f) * scale, Vector3(0, 1, 0), Vector2(1.f, 1.f)},
		//-y
		Vertex{Vector3(-0.5f, -0.5f, -0.5f) * scale, Vector3(0, -1, 0), Vector2(0.f, 1.f)},
		Vertex{Vector3(0.5f, -0.5f, -0.5f) * scale, Vector3(0, -1, 0), Vector2(0.f, 0.f)},
		Vertex{Vector3(0.5f, -0.5f, 0.5f) * scale, Vector3(0, -1, 0), Vector2(1.f, 0.f)},
		Vertex{Vector3(-0.5f, -0.5f, 0.5f) * scale, Vector3(0, -1, 0), Vector2(1.f, 1.f)},
		//+z
		Vertex{Vector3(0.5f, -0.5f, 0.5f) * scale, Vector3(0, 0, 1), Vector2(0.f, 1.f)},
		Vertex{Vector3(0.5f, 0.5f, 0.5f) * scale, Vector3(0, 0, 1), Vector2(0.f, 0.f)},
		Vertex{Vector3(-0.5f, 0.5f, 0.5f) * scale, Vector3(0, 0, 1), Vector2(1.f, 0.f)},
		Vertex{Vector3(-0.5f, -0.5f, 0.5f) * scale, Vector3(0, 0, 1), Vector2(1.f, 1.f)},
		//+x
		Vertex{Vector3(0.5f, -0.5f, -0.5f) * scale, Vector3(1, 0, 0), Vector2(0.f, 1.f)},
		Vertex{Vector3(0.5f, 0.5f, -0.5f) * scale, Vector3(1, 0, 0), Vector2(0.f, 0.f)},
		Vertex{Vector3(0.5f, 0.5f, 0.5f) * scale, Vector3(1, 0, 0), Vector2(1.f, 0.f)},
		Vertex{Vector3(0.5f, -0.5f, 0.5f) * scale, Vector3(1, 0, 0), Vector2(1.f, 1.f)},
		//-z
		Vertex{Vector3(-0.5f, -0.5f, -0.5f) * scale, Vector3(0, 0, -1), Vector2(0.f, 1.f)},
		Vertex{Vector3(-0.5f, 0.5f, -0.5f) * scale, Vector3(0, 0, -1), Vector2(0.f, 0.f)},
		Vertex{Vector3(0.5f, 0.5f, -0.5f) * scale, Vector3(0, 0, -1), Vector2(1.f, 0.f)},
		Vertex{Vector3(0.5f, -0.5f, -0.5f) * scale, Vector3(0, 0, -1), Vector2(1.f, 1.f)},
		//-x			
		Vertex{Vector3(-0.5f, -0.5f, 0.5f) * scale, Vector3(-1, 0, 0), Vector2(0.f, 1.f)},
		Vertex{Vector3(-0.5f, 0.5f, 0.5f) * scale, Vector3(-1, 0, 0), Vector2(0.f, 0.f)},
		Vertex{Vector3(-0.5f, 0.5f, -0.5f) * scale, Vector3(-1, 0, 0), Vector2(1.f, 0.f)},
		Vertex{Vector3(-0.5f, -0.5f, -0.5f) * scale, Vector3(-1, 0, 0), Vector2(1.f, 1.f)},
	};
	vector<uint32_t> v_index =
	{ 0,1,2,0,2,3,
	4,5,6,4,6,7,
8,9,10,8,10,11,
12,13,14,12,14,15,
16,17,18,16,18,19,
20,21,22,20,22,23 };
	return MeshData(v_vert, v_index);

}
MeshData MeshFactory::CreateSphere(const float radius, const int numSlices, const int numStacks)
{
	const float dTheta = -XM_2PI / float(numSlices);
	const float dPhi = -XM_PI / float(numStacks);

	vector<Vertex> vertices;

	for (int j = 0; j <= numStacks; j++) {

		// 스택에 쌓일 수록 시작점을 x-y 평면에서 회전 시켜서 위로 올리는 구조
		Vector3 stackStartPoint = Vector3::Transform(
			Vector3(0.0f, -radius, 0.0f), Matrix::CreateRotationZ(dPhi * j));

		for (int i = 0; i <= numSlices; i++) {
			Vertex v;

			// 시작점을 x-z 평면에서 회전시키면서 원을 만드는 구조
			v.pos = Vector3::Transform(
				stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));

			v.normal = v.pos; // 원점이 구의 중심
			v.normal.Normalize();
			v.uv =
				Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks);

			vertices.push_back(v);
		}
	}

	// cout << vertices.size() << endl;

	vector<uint32_t> indices;

	for (int j = 0; j < numStacks; j++) {

		const int offset = (numSlices + 1) * j;

		for (int i = 0; i < numSlices; i++) {

			indices.push_back(offset + i);
			indices.push_back(offset + i + numSlices + 1);
			indices.push_back(offset + i + 1 + numSlices + 1);

			indices.push_back(offset + i);
			indices.push_back(offset + i + 1 + numSlices + 1);
			indices.push_back(offset + i + 1);
		}
	}
	return MeshData(vertices, indices);

}

//vector<Mesh> MeshFactory::ReadFromFile(std::string basePath, std::string filename) {
//
//	using namespace DirectX;
//
//	ModelLoader modelLoader;
//	modelLoader.Load(basePath, filename);
//	vector<Mesh>& meshes = modelLoader.meshes;
//
//	// Normalize vertices
//	Vector3 vmin(1000, 1000, 1000);
//	Vector3 vmax(-1000, -1000, -1000);
//	for (auto& mesh : meshes) {
//		for (auto& v : mesh.vertices) {
//			vmin.x = XMMin(vmin.x, v.pos.x);
//			vmin.y = XMMin(vmin.y, v.pos.y);
//			vmin.z = XMMin(vmin.z, v.pos.z);
//			vmax.x = XMMax(vmax.x, v.pos.x);
//			vmax.y = XMMax(vmax.y, v.pos.y);
//			vmax.z = XMMax(vmax.z, v.pos.z);
//		}
//	}
//
//	float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
//	float dl = XMMax(XMMax(dx, dy), dz);
//	float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
//		cz = (vmax.z + vmin.z) * 0.5f;
//
//	for (auto& mesh : meshes) {
//		for (auto& v : mesh.vertices) {
//			v.pos.x = (v.pos.x - cx) / dl;
//			v.pos.y = (v.pos.y - cy) / dl;
//			v.pos.z = (v.pos.z - cz) / dl;
//		}
//	}
//
//	return meshes;
//}