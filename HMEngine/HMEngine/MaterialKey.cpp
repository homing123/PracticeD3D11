#include "BlinnPhongMaterial.h"
#include "IBLMaterial.h"
#include "MousePickingMaterial.h"
#include "BillboardPointImageMaterial.h"
#include "SkyboxMaterial.h"

const size_t makeHash(const string& str)
{
	return hash<string>()(str);
}

const size_t BlinnPhongMaterial::ambientKey = makeHash("ambient");
const size_t BlinnPhongMaterial::diffuseKey = makeHash("diffuse");
const size_t BlinnPhongMaterial::shininessKey = makeHash("shininess");
const size_t BlinnPhongMaterial::specularKey = makeHash("specular");

const size_t IBLMaterial::diffuseKey = makeHash("diffuse");
const size_t IBLMaterial::specularKey = makeHash("specular");

const size_t MousePickingMaterial::idxKey = makeHash("idx");

const size_t BillboardPointImageMaterial::idxKey = makeHash("idx");


