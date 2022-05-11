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