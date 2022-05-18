#pragma once

#include "Base.h"

/* Ư�� �ִϸ��̼��� �����ϴµ� �ʿ��� ���� �ִϸ��̼� ����. */
/* �ִϸ��̼� ���� �ϳ��� ���� ���� Channel�� ������ ������� �� �մ�. */

BEGIN(Engine)

class CChannel final :  public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;


public:
	void SetUp_HierarchyNodePtr(class HierarchyNode* pNode) {
		m_pHierarchyNode = pNode;
		Safe_AddRef(m_pHierarchyNode);
	}

	void Set_CurrentKeyFrame(_uint iKeyFrame) {
		m_iCurrentKeyFrame = iKeyFrame;
	}


public:
	HRESULT NativeConstruct(aiNodeAnim* pAIChannel);
	void Compute_TransformationMatrix(_double PlayTime);

private:
	char				m_szName[MAX_PATH] = "";

private:
	_uint							m_iNumKeyFrames = 0;
	_uint							m_iCurrentKeyFrame = 0;


	vector<KEYFRAME*>				m_KeyFrames;
	typedef vector<KEYFRAME*>		KEYFRAMES;

	class HierarchyNode*			m_pHierarchyNode = nullptr;


private:
	_float4x4	m_TransformationMatrix;
private:
	HRESULT	Ready_KeyFrames(aiNodeAnim* pAIChannel);
	
public:
	static CChannel* Create(aiNodeAnim* pAIChannel);
	virtual void Free() override;
};

END