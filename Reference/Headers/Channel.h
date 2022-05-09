#pragma once

#include "Base.h"

/* Ư�� �ִϸ��̼��� �����ϴµ� �ʿ��� ���� �ִϸ��̼� ����. */
/* �ִϸ��̼� ���� �ϳ��� ���� ���� Channel�� ������ ������� �� �մ�. */

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