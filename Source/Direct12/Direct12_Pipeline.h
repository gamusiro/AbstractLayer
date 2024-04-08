/*===================================================================================
*    Date : 2022/12/04(日)
*        Author		: Gakuto.S
*        File		: Direct12_Pipeline.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_Pipeline.h"

class Direct12Pipeline : public IPipeline
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	Direct12Pipeline(
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipeline,
		D3D12_PRIMITIVE_TOPOLOGY primitive,
		std::map<size_t, ID3D12Resource*> shaderResources,
		std::map<size_t, ID3D12DescriptorHeap*> textureHeaps
	)
		:m_rootSignature(rootSignature),
		m_pipeline(pipeline),
		m_primitive(primitive),
		m_shaderResources(shaderResources),
		m_setHeaps(),
		m_textureHeaps(textureHeaps)
	{
		for (auto& heap : m_textureHeaps)
			m_setHeaps.push_back(heap.second);
	}

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~Direct12Pipeline()
	{
		m_shaderResources.clear();
		SAFE_RELEASE(m_rootSignature);
		SAFE_RELEASE(m_pipeline);
	}

	////////		Direct12に依存するメソッド群		////////
public:
	//**************************************************
	/// \brief ルートシグネチャの取得
	/// 
	/// \return ルートシグネチャ
	//**************************************************
	ID3D12RootSignature* GetRootSignature() { return m_rootSignature; }

	//**************************************************
	/// \brief パイプラインステートの取得
	/// 
	/// \return パイプラインステート
	//**************************************************
	ID3D12PipelineState* GetPipelineState() { return m_pipeline; }

	//**************************************************
	/// \brief プリミティブトポロジーの取得
	/// 
	/// \return プリミティブトポロジー
	//**************************************************
	D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() { return m_primitive; }

	//**************************************************
	/// \brief シェーダリソースの取得
	/// 
	/// \return パイプラインステート
	//**************************************************
	std::map<size_t, ID3D12Resource*> GetShaderResources() { return m_shaderResources; }

	//**************************************************
	/// \brief セットするヒープの取得
	/// 
	/// \return ヒープ
	//**************************************************
	std::vector<ID3D12DescriptorHeap*> GetSetHeaps() { return m_setHeaps; }

	//**************************************************
	/// \brief テクスチャヒープの取得
	/// 
	/// \return テクスチャヒープ
	//**************************************************
	std::map<size_t, ID3D12DescriptorHeap*> GetTextureHeaps() { return m_textureHeaps; }

private:
	ID3D12RootSignature*				m_rootSignature;
	ID3D12PipelineState*				m_pipeline;
	D3D12_PRIMITIVE_TOPOLOGY			m_primitive;

	std::map<size_t, ID3D12Resource*>	m_shaderResources;
	
	std::vector<ID3D12DescriptorHeap*>		m_setHeaps;
	std::map<size_t, ID3D12DescriptorHeap*>	m_textureHeaps;
};

