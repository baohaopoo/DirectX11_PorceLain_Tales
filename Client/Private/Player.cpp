#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"

Player::Player(ID3D11Device * pDeviceOut, ID3D11DeviceContext * pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

Player::Player(const Player & rhs)
	: CGameObject(rhs)
{
}

HRESULT Player::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Player::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);


	//CTransform* m_pTransform = nullptr; /*(CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_UIObject"), TEXT("Com_Transform"));*/
	//m_pTransform->Set_State(CTransform::STATE_RIGHT, {-90.f, 0.f,0.f});

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	return S_OK;
}

void Player::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	KeyInput(TimeDelta);

}

void Player::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT Player::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	//dx9과 다른 bind 방식 - 주요 차이점이다.
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		//Diffuse 는 한마디 텍스쳐 전체다. 
		//if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
		//	return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(2)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void Player::KeyInput(float TimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	Safe_AddRef(pGameInstance);
	if (pGameInstance->Get_DIKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(TimeDelta);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_BackWard(TimeDelta);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Go_Left(TimeDelta);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Go_Right(TimeDelta);
	}
	Safe_Release(pGameInstance);
}

HRESULT Player::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	//월드 매트릭스 바인딩.
	return S_OK;
}

HRESULT Player::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPositionFloat4(), sizeof(_float4))))
		return E_FAIL;

	/* For.LightDesc */
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT Player::Create_MaskTexture()
{
	return E_NOTIMPL;
}

Player * Player::Create(ID3D11Device * pDeviceOut, ID3D11DeviceContext * pDeviceContextOut)
{
	Player*	pInstance = new Player(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * Player::Clone(void * pArg)
{
	Player*	pInstance = new Player(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Player::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
