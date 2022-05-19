#pragma once

#include "Base.h"


BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT NativeConstruct(aiAnimation* pAIAnimation, vector<class CHierarchyNode*> Nodes);
	void Update(_double TimeDelta);

private:
	char					m_szName[MAX_PATH] = "";
	_double					m_Duration = 0.0; /* 애니메이션 하나를 재생하는데 걸리는 총 시간. */
	_double					m_TickPerSecond = 0.0;

	_double					m_TimeAcc = 0.0;
	_bool					m_isFinished = false;

private:
	_uint								m_iNumChannels = 0;

	vector<class CChannel*>				m_Channels;
	typedef vector<class CChannel*>		CHANNELS;

private:
	HRESULT Ready_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);



public:
	static CAnimation* Create(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	virtual void Free() override;
};

END