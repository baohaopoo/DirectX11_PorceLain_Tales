#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT NativeConstruct(aiAnimation* pAnimationName);
	void Update(_double TimeDelta);


private:
	char					m_szName[MAX_PATH] = "";
	_double					m_Duration = 0.0;		//애니메이션 돌리는 기간
	_double					m_TickPerSecond = 0.0;	//초당


private:

	_uint								m_iNumChannels = 0;
	vector<class CChannel*>				m_Channels;
	typedef vector<class CChannel*>		CHANNELS;

private:
	HRESULT	Ready_Channels(aiAnimation* pAIAnimation);
public:
	static CAnimation* Create(aiAnimation* pAIAnimation);
	virtual void Free() override;
};

END