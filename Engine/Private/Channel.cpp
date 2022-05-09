#include "..\Public\Channel.h"

CChannel::CChannel()
{

}

HRESULT CChannel::NativeConstruct(const char * pChannelName)
{
	return S_OK;
}

CChannel * CChannel::Create(const char * pChannelName)
{
	CChannel*	pInstance = new CChannel();

	if (FAILED(pInstance->NativeConstruct(pChannelName)))
	{
		MSG_BOX(TEXT("Failed to Created CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	for (auto pKeyFrame : m_KeyFrames)
		Safe_Delete(pKeyFrame);
 }
