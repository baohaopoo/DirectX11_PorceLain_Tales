#include "stdafx.h"
#include "ExitBtn.h"
#include "GameInstance.h"

ExitBtn::ExitBtn(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

ExitBtn::ExitBtn(const ExitBtn& rhs)
	: CGameObject(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT ExitBtn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	//직교 투영 부분.
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));

	return S_OK;
}

HRESULT ExitBtn::NativeConstruct(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//위치
	m_fX = 233.f;
	m_fY = 100.f;

	//사이즈
	m_fSizeX = 100;
	m_fSizeY = 100;

	return S_OK;
}

void ExitBtn::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	//여기서 텍스처의 컴포넌트를 바꿔주려고 했었다.
	changeTexture();
	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f, 1.f));

	RECT rc{ 183,50,283,150 };
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&rc, pt)) {

		//모양만 바꾸기
		isChange = true;
		//여기서 크기 조절.
		m_pTransformCom->Scaled(_float3(m_fSizeY + 30, m_fSizeY + 30, 1.f));


		//우클릭을 아예 해버린다면, 
		if (GetAsyncKeyState(VK_LBUTTON)) {
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);




			RELEASE_INSTANCE(CGameInstance);
		}

	}
	else m_pTransformCom->Scaled(_float3(m_fSizeY , m_fSizeY , 1.f));


}

void ExitBtn::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT ExitBtn::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;


	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	_float4x4	ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float4x4	ProjMatrixTP;
	XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void ExitBtn::changeTexture()
{
	if (isChange)
	{
		/* For.Com_Texture */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_LOGO, TEXT("Prototype_Component_Texture_PressPlayBtnUI"), (CComponent**)&m_pTextureCom)))
			return;
		
	}
	else {
		/* For.Com_Texture */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_LOGO, TEXT("Prototype_Component_Texture_PlayBtnUI"), (CComponent**)&m_pTextureCom)))
			return;
	}

}

HRESULT ExitBtn::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


 



	return S_OK;
}

ExitBtn* ExitBtn::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	ExitBtn* pInstance = new ExitBtn(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created ExitBtn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* ExitBtn::Clone(void* pArg)
{
	ExitBtn* pInstance = new ExitBtn(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created ExitBtn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ExitBtn::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
