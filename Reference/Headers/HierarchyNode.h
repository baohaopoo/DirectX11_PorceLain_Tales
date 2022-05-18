#pragma once
#include "Base.h"

BEGIN(Engine)

class HierarchyNode final : public CBase
{
private:
	HierarchyNode();
	virtual ~HierarchyNode()=default;
public:
	const char* Get_Name()
	{
		return m_szName;
	}

	_uint Get_Depth() const {
		return m_iDepth;
	}

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombinedMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

public:
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}
public:
	HRESULT NativeConstruct(aiNode* pAINode, HierarchyNode* pParent, _uint iDepth);
	void Update_CombinedTransformationMatrix();
private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iDepth = 0;
	HierarchyNode*	m_pParent = nullptr;
	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformationMatrix; /* 부모기준 나의 상태. */
	_float4x4		m_CombinedTransformationMatrix; /* 나의 상태  * 부모상태 */

public:
	static HierarchyNode* Create(aiNode* pAINode, HierarchyNode* pParent, _uint iDepth);
	virtual void Free() override;

};
END