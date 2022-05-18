#include "..\Public\Channel.h"
#include "HierarchyNode.h"

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

void CChannel::Compute_TransformationMatrix(_double PlayTime)
{
	while (m_iCurrentKeyFrame + 1 < m_KeyFrames.size())
	{
		if (PlayTime > m_KeyFrames[m_iCurrentKeyFrame + 1]->Time)
			++m_iCurrentKeyFrame;
		else
			break;
	}

	KEYFRAME*	pLastKeyFrame = m_KeyFrames.back();

	_vector		vScale, vRotation, vTranslation;

	if (PlayTime > pLastKeyFrame->Time)
	{
		vScale = XMLoadFloat3(&pLastKeyFrame->vScale);
		vRotation = XMLoadFloat4(&pLastKeyFrame->vRotation);
		vTranslation = XMLoadFloat3(&pLastKeyFrame->vTranslation);
		vTranslation = XMVectorSetW(vTranslation, 1.f);
	}

	else
	{
		_double		Ratio = (PlayTime - m_KeyFrames[m_iCurrentKeyFrame]->Time) / (m_KeyFrames[m_iCurrentKeyFrame + 1]->Time - m_KeyFrames[m_iCurrentKeyFrame]->Time);

		_vector		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame]->vScale);
		_vector		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame]->vRotation);
		_vector		vSourTranslation = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame]->vTranslation);

		_vector		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vScale);
		_vector		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vRotation);
		_vector		vDestTranslation = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vTranslation);

		vScale = XMVectorLerp(vSourScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, (_float)Ratio);
		vTranslation = XMVectorSetW(vTranslation, 1.f);
	}

	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation));

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

		//scale이나 rotation값이 0일수도 있기 때문에.. 
		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			pKeyFrame->Time = pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{

			//rotation은 쿼터니언이라서 x,y,z,w순이 아니라 w,x,y,z순ㄴ이라서 하나씩 다 채워야함.
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
