#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{

}

HRESULT CAnimation::NativeConstruct(const char * pAnimationName)
{
	strcpy_s(m_szName, pAnimationName);

	return S_OK;
}

CAnimation * CAnimation::Create(const char * pAnimationName)
{
	CAnimation*	pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct(pAnimationName)))
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

