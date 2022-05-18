#include "..\Public\HierarchyNode.h"



HierarchyNode::HierarchyNode()
{
}

HRESULT HierarchyNode::NativeConstruct(aiNode * pAINode, HierarchyNode * pParent, _uint iDepth)
{
	strcpy_s(m_szName, pAINode->mName.data);

	m_pParent = pParent;

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	m_iDepth = iDepth;

	return S_OK;
}

void HierarchyNode::Update_CombinedTransformationMatrix()
{
	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix));
}

HierarchyNode * HierarchyNode::Create(aiNode * pAINode, HierarchyNode* pParent, _uint iDepth)
{
	HierarchyNode*	pInstance = new HierarchyNode();


	if (FAILED(pInstance->NativeConstruct(pAINode, pParent, iDepth)))
	{
		MSG_BOX(TEXT("Failed to Created HierarchyNode"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void HierarchyNode::Free()
{
}


