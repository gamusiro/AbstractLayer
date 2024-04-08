/*===================================================================================
*    Date : 2022/12/11(日)
*        Author		: Gakuto.S
*        File		: gltf_loader.h
*        Detail		:
===================================================================================*/
#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <array>
#include <DirectXMath.h>

struct GLTF_MATERIAL
{
	std::array<float, 3>		emissive;
	std::array<float, 4>		color;
	float						metallic;
	float						roughness;
	std::vector<unsigned char>	baseColorTextureMeta;
};

struct GLTF_PRIMITIVE
{
	std::vector<std::array<float, 3>> pos;
	std::vector<std::array<float, 3>> nor;
	std::vector<std::array<float, 2>> tex0;

	int perIndexSize;
	std::vector<unsigned char> indices;

	GLTF_MATERIAL material;
};

struct GLTF_LOCAL
{
	std::vector<double> translate;
	std::vector<double> rotate;
	std::vector<double> scale;
};


struct GLTF_MESH
{
	std::vector<GLTF_PRIMITIVE> primitives;
	GLTF_LOCAL					localeParam;
	std::string					parentMeshName;
};

class GLTFLoader
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \param[in] fileName
	/// \param[in] binary
	/// 
	/// \return なし
	//**************************************************
	GLTFLoader(const char* fileName, bool binary = false);

	//**************************************************
	/// \brief プリミティブデータの取得
	/// 
	/// \return なし
	//**************************************************
	std::map<std::string, GLTF_MESH> GetMeshes() { return m_meshes; }

private:
	std::map<std::string, GLTF_MESH>	m_meshes;
};
