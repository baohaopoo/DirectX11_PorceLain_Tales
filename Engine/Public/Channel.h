#pragma once

#include "Base.h"

/* 특정 애니메이션을 동작하는데 필요한 뼈의 애니메이션 정보. */
/* 애니메이션 마다 하나의 뼈에 대한 Channel이 여러개 만들어질 수 잇다. */

BEGIN(Engine)

class CChannel final :  public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT NativeConstruct(aiNodeAnim* pAIChannel);

private:
	char				m_szName[MAX_PATH] = "";

private:
	_uint							m_iNumKeyFrames = 0;
	vector<KEYFRAME*>				m_KeyFrames;
	typedef vector<KEYFRAME*>		KEYFRAMES;

private:
	HRESULT	Ready_KeyFrames(aiNodeAnim* pAIChannel);
	
public:
	static CChannel* Create(aiNodeAnim* pAIChannel);
	virtual void Free() override;
};

END