#include "..\Public\Animation.h"
#include "Channel.h"
#include "HierarchyNode.h"

CAnimation::CAnimation()
{

}

HRESULT CAnimation::NativeConstruct(aiAnimation * pAIAnimation, vector<HierarchyNode*> Nodes)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_Duration = pAIAnimation->mDuration;
	m_TickPerSecond = pAIAnimation->mTicksPerSecond;

	if (FAILED(Ready_Channels(pAIAnimation, Nodes)))
		return E_FAIL;

	return S_OK;
}

void CAnimation::Update(_double TimeDelta)
{/* 1. ���� �� �ִϸ��̼��� ����ǰ� �ִ� �ð��� ����Ѵ�. */
	/* 2. �� �ð��뿡 �´� Ű�������� ã�´�. */
	/* 3. �ش� Ű�����ӻ����� ���� ��İ��� ���������Ͽ� ���Ѵ�.  */
	/* 4. �̷��� ���� ����� ��忡 �����صд�. */

	m_TimeAcc += m_TickPerSecond * TimeDelta;

	if (m_TimeAcc >= m_Duration)
	{
		m_isFinished = true;
		m_TimeAcc = 0.0;
	}

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (true == m_isFinished)
			m_Channels[i]->Set_CurrentKeyFrame(0);

		m_Channels[i]->Compute_TransformationMatrix(m_TimeAcc);

	}
}

CAnimation * CAnimation::Create(aiAnimation* pAIAnimation, vector<HierarchyNode*> Nodes)
{

	CAnimation*	pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct(pAIAnimation,Nodes)))
	{
		MSG_BOX(TEXT("Failed to Created CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CAnimation::Ready_Channels(aiAnimation * pAIAnimation, vector<HierarchyNode*> Nodes)
{
	m_iNumChannels = pAIAnimation->mNumChannels;
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim* pAIChannel = pAIAnimation->mChannels[i];

		CChannel* pChannel = CChannel::Create(pAIChannel);
		if (nullptr == pChannel)
			return E_FAIL;

		auto	iter = find_if(Nodes.begin(), Nodes.end(), [&](HierarchyNode* pNode)
		{
			return !strcmp(pAIChannel->mNodeName.data, pNode->Get_Name());
		});
		if (iter == Nodes.end())
			return E_FAIL;

		pChannel->SetUp_HierarchyNodePtr(*iter);

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

CAnimation * CAnimation::Create(aiAnimation * pAIAnimation, vector<HierarchyNode*> Nodes)
{
	CAnimation* pInstance = new CAnimation();
	if (FAILED(pInstance->NativeConstruct(pAIAnimation, Nodes)))
	{
		MSG_BOX(TEXT("Failed to Created CAnimation"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}

