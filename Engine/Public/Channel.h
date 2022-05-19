#pragma once

#include "Base.h"

/* Ư�� �ִϸ��̼��� �����ϴµ� �ʿ��� ���� �ִϸ��̼� ����. */
/* �ִϸ��̼� ���� �ϳ��� ���� ���� Channel�� ������ ������� �� �մ�. */

BEGIN(Engine)

class CChannel final :  public CBase
{
private:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;

public:
	void SetUp_HierarchyNodePtr(class CHierarchyNode* pNode) {
		m_pHierarchyNode = pNode;
		Safe_AddRef(m_pHierarchyNode);
	}

	void Set_CurrentKeyFrame(_uint iKeyFrame) {
		m_iCurrentKeyFrame = iKeyFrame;
	}

public:
	HRESULT NativeConstruct_Prototype(aiNodeAnim* pAIChannel);
	HRESULT NativeConstruct();
	void Compute_TransformationMatrix(_double PlayTime);

private:
	char				m_szName[MAX_PATH] = "";

private:
	_uint							m_iNumKeyFrames = 0;
	_uint							m_iCurrentKeyFrame = 0;

	vector<KEYFRAME*>				m_KeyFrames;
	typedef vector<KEYFRAME*>		KEYFRAMES;

	class CHierarchyNode*			m_pHierarchyNode = nullptr;

private:
	_float4x4						m_TransformationMatrix;
	_bool							m_isCloned = false;

private:

	HRESULT Ready_KeyFrames(aiNodeAnim* pAIChannel);

public:
	static CChannel* Create(aiNodeAnim* pAIChannel);
	CChannel* Clone();
	virtual void Free() override;
};

END