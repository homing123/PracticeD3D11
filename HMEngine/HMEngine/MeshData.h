#pragma once
#include <vector>
#include <string>
#include "Vertex.h"
using namespace std;
struct MeshData
{
	MeshData(vector<Vertex> v, vector<uint32_t>i) : vertices(v), indices(i) {};
	vector<Vertex> vertices;
	vector<uint32_t> indices;

	string albedoTexFilename;
	string emissiveTexFilename;
	string normalTexFilename;
	string heightTexFilename;
	string aoTexFilename;
	string metalicTexFilename;
	string roughnessTexFilename;
};