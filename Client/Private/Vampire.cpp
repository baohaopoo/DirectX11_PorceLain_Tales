#include "stdafx.h"
#include "..\Public\Vampire.h"
#include "GameInstance.h"

CVampire::CVampire(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CVampire::CVampire(const CVampire & rhs)
	: CGameObject(rhs)
{
}

HRESULT CVampire::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVampire::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	



	return S_OK;
}

void CVampire::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CVampire::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CVampire::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(2)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}	

	return S_OK;
}

HRESULT CVampire::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CVampire::SetUp_ConstantTable()
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

void CVampire::KeyInput(float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	Safe_AddRef(pGameInstance);
	if (pGameInstance->Get_DIKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_BackWard(fTimeDelta);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	Safe_Release(pGameInstance);
}

CVampire * CVampire::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CVampire*	pInstance = new CVampire(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CFork"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CVampire::Clone(void * pArg)
{
	CVampire*	pInstance = new CVampire(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVampire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVampire::Free()
{
	__super::Free();	
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
