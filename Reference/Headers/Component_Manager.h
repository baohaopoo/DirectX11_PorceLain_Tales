#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"


/* 컴포넌트들의 원형을 모아둔다. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);

private:
	_uint											m_iNumLevels = 0;
	map<const _tchar*, CComponent*>*				m_pPrototypes = nullptr;
	typedef map<const _tchar*, CComponent*>			PROTOTYPES;

private:
	CComponent* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END