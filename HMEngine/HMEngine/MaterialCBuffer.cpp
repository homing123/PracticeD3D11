#include "MaterialCBuffer.h"

const size_t makeHash(const string& str)
{
	return hash<string>()(str);
}

const size_t BlinnPhongCBuffer::ambientKey = makeHash("ambient");
const size_t BlinnPhongCBuffer::diffuseKey = makeHash("diffuse");
const size_t BlinnPhongCBuffer::shininessKey = makeHash("shininess");
const size_t BlinnPhongCBuffer::specularKey = makeHash("specular");

const size_t IBLCBuffer::diffuseKey = makeHash("diffuse");
const size_t IBLCBuffer::specularKey = makeHash("specular");

const size_t MousePickingCBuffer::idxKey = makeHash("idx");
