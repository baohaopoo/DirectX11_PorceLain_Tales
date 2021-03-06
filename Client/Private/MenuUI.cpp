#include "stdafx.h"
#include "MenuUI.h"
#include "GameInstance.h"
MenuUI::MenuUI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
    : CGameObject(pDeviceOut, pDeviceContextOut)
{
}

MenuUI::MenuUI(const MenuUI& rhs)
    : CGameObject(rhs)
    , m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT MenuUI::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    //직교 투영 부분.
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));

    return S_OK;
}

HRESULT MenuUI::NativeConstruct(void* pArg)
{
    CTransform::TRANSFORMDESC		TransformDesc;
    TransformDesc.SpeedPerSec = 5.0f;
    TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

    if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
        return E_FAIL;

    if (FAILED(SetUp_Components()))
        return E_FAIL;

    ////위치
    //m_fX = 233.f;
    //m_fY = 300.f;

    ////사이즈
    //m_fSizeX = 255;
    //m_fSizeY = 600;


    m_fX = g_iWinCX * 0.5f;
    m_fY = g_iWinCY * 0.5f;

    m_fSizeX = g_iWinCX;
    m_fSizeY = g_iWinCY;


    return S_OK;
}

void MenuUI::Tick(_double TimeDelta)
{
    __super::Tick(TimeDelta);


    m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f, 1.f));
}

void MenuUI::LateTick(_double TimeDelta)
{
    __super::LateTick(TimeDelta);

    if (nullptr != m_pRendererCom)
        m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT MenuUI::Render()
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

HRESULT MenuUI::SetUp_Components()
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

    /* For.Com_Texture */
    if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_LOGO, TEXT("Prototype_Component_Texture_MenuUI"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

MenuUI* MenuUI::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
    MenuUI* pInstance = new MenuUI(pDeviceOut, pDeviceContextOut);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created UI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* MenuUI::Clone(void* pArg)
{
    MenuUI* pInstance = new MenuUI(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created MenuUI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void MenuUI::Free()
{
    __super::Free();
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
