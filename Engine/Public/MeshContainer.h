#pragma once

/* 모델의 한 파츠. */
/* 해당 파츠를 그리기위한 정점, 인덱스 버퍼를 가진다. */
#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMeshContainer final : public CVIBuffer
{
private:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CMeshContainer() = default;

public:
	_uint Get_NumMaterialIndex() const {
		return m_iMaterialIndex;
	} 

public:
	HRESULT NativeConstruct_Prototype(CModel::TYPE eType, aiMesh* pAIMesh, _float4x4 PivotMatrix,vector<HierarchyNode*>	HierarchyNodes);

public:
	HRESULT Create_VertexBuffer_NonAnim(aiMesh* pAIMesh);
	HRESULT Create_VertexBuffer_Anim(aiMesh* pAIMesh, vector<HierarchyNode*>	HierarchyNodes);
	HRESULT Create_SkinnedInfo(aiMesh* pAIMesh, VTXANIMMODEL* pVertices, vector<HierarchyNode*>	HierarchyNodes);
	HRESULT Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);


private:
	CModel::TYPE				m_eType = CModel::TYPE_END;
	_float4x4					m_PivotMatrix;
	_uint						m_iMaterialIndex = 0;


private:
	_uint								m_iNumBones = 0;
	vector <class HierarchyNode*>		m_MeshContainerNodes;

private:
	class HierarchyNode*				m_pHierarchyNode = nullptr;


public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CModel::TYPE eType, aiMesh* pAIMesh, _float4x4 PivotMatrix, vector<class HierarchyNode*> HierarchyNodes);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END