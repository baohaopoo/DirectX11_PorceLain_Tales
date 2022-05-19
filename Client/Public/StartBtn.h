#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)


class StartBtn final : public CGameObject
{
private:
	explicit StartBtn(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit StartBtn(const StartBtn& rhs);
	virtual ~StartBtn() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	void changeTexture();
private:
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

private:

	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	HRESULT SetUp_Components();
private:
	bool isChange = false;
public:
	void ChangeBtn();
public:
	static StartBtn* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;
};

END