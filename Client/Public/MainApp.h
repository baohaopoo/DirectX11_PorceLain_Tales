#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
public:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT NativeConstruct();
	void Tick(_double TimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	CRenderer*				m_pRenderer = nullptr;



public:
	HRESULT Open_Level(LEVEL eLevelIndex);
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Prototype_Component();	

public:
	//UI ���� ������Ʈ �̸� �ε� 
	HRESULT Ready_Prototype_GameObject_UI();
	//Texture �̸� �ε�
	HRESULT Ready_Prototype_Texture();
	HRESULT Ready_Prototype_Texture_UI();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END



