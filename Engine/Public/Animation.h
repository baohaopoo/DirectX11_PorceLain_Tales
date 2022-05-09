#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT NativeConstruct(const char* pAnimationName);

private:
	char					m_szName[MAX_PATH] = "";

private:
	vector<class CChannel*>				m_Channels;
	typedef vector<class CChannel*>		CHANNELS;

public:
	static CAnimation* Create(const char* pAnimationName);
	virtual void Free() override;
};

END