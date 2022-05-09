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
	HRESULT NativeConstruct(const char* pChannelName);

private:
	char				m_szName[MAX_PATH] = "";

private:
	vector<KEYFRAME*>				m_KeyFrames;
	typedef vector<KEYFRAME*>		KEYFRAMES;

public:
	static CChannel* Create(const char* pChannelName);
	virtual void Free() override;
};

END