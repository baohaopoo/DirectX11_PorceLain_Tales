#include "..\Public\Channel.h"

CChannel::CChannel()
{

}

HRESULT CChannel::NativeConstruct(aiNodeAnim* pAIChannel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);
	if (FAILED(Ready_KeyFrames(pAIChannel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChannel::Ready_KeyFrames(aiNodeAnim * pAIChannel)
{
	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3 vScale;
	_float4 vRotation;
	_float3 vTranslation;


	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME*			pKeyFrame = new KEYFRAME;
		ZeroMemory(pKeyFrame, sizeof(KEYFRAME));

		//scale�̳� rotation���� 0�ϼ��� �ֱ� ������.. 
		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			pKeyFrame->Time = pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{

			//rotation�� ���ʹϾ��̶� x,y,z,w���� �ƴ϶� w,x,y,z�����̶� �ϳ��� �� ä������.
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			pKeyFrame->Time = pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			pKeyFrame->Time = pAIChannel->mPositionKeys[i].mTime;
		}

		pKeyFrame->vScale = vScale;
		pKeyFrame->vRotation = vRotation;
		pKeyFrame->vTranslation = vTranslation;

		m_KeyFrames.push_back(pKeyFrame);
	}

	return S_OK;

}

CChannel * CChannel::Create(aiNodeAnim* pAIChannel)
{
	CChannel*	pInstance = new CChannel();

	if (FAILED(pInstance->NativeConstruct(pAIChannel)))
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
