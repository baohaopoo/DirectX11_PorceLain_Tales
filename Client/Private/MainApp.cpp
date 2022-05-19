#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Level_Logo.h"
#include "Level_Loading.h"


//UI용 헤더
#include "UI.h"
#include "MenuUI.h"
#include "ExitBtn.h"
#include "ExitUI.h"
#include "LoadingUI.h"
#include "EyeUI.h"
#include "HeartUI.h"
#include "StartBtn.h"
CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);

	
	// D3D11_DEPTH_STENCIL_DESC
	// D3D11_BLEND_DESC

	
	
	/*D3D11_RASTERIZER_DESC
	D3D11_DEPTH_STENCIL_DESC
	D3D11_BLEND_DESC*/

		//m_pDeviceContext->OMSetBlendState(0xffffffff);


	/*
	m_pDeviceContext->OMSetDepthStencilState();
	m_pDeviceContext->RasterizerSTate();*/
}

HRESULT CMainApp::NativeConstruct()
{
	/*D3D11_SAMPLER_DESC*/

	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;

	/* 장치초기화 등. 엔진의 기능을 활용하기위한 기초 초기화 작업을 마친다. */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	//텍스쳐용
	if (FAILED(Ready_Prototype_Texture()))
		return E_FAIL;


	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_pGameInstance->Tick_Engine(TimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin(_float4(0.0f, 0.f, 1.f, 1.f));	

	if (FAILED(m_pRenderer->Render_GameObjects()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel*			pLevel = nullptr;

	switch (eLevelIndex)
	{
	case LEVEL_LOGO:
		pLevel = CLevel_Logo::Create(m_pDevice, m_pDeviceContext);
		break;

	default:
		pLevel = CLevel_Loading::Create(m_pDevice, m_pDeviceContext, (LEVEL)eLevelIndex);
		break;
	}

	if (FAILED(m_pGameInstance->Open_Level(eLevelIndex, pLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), 
		CBackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(Ready_Prototype_GameObject_UI())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_UI"));
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_UI()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"), UI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuUI"), MenuUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExitBtnUI"), ExitBtn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExitUI"), ExitUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//진짜 ExitBtn
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StartBtnUI"), StartBtn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingUI"), LoadingUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EyeUI"), EyeUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HeartUI"), HeartUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture()
{
	// UI용 텍스쳐 올리기.
	if (FAILED(Ready_Prototype_Texture_UI())) {
		MSG_BOX(TEXT("Failed to Load Texture UI"));
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture_UI()
{
	/* For.Prototype_Component_Texture_Default  UI2*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_LogoUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/LogoUI/Start.dds"), 1))))
		return E_FAIL;


	/* Menu UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_MenuUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/LogoUI/Start2.dds"), 1))))
		return E_FAIL;




	/* ExitBtnUI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_ExitBtnUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/LogoUI/ExitBtn.dds"), 1))))
		return E_FAIL;

	/* PressExitBtnUI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_PressExitBtnUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/LogoUI/PressExitBtn.dds"), 1))))
		return E_FAIL;

	/*PlayBtnUI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_PlayBtnUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/LogoUI/PlayBtn.dds"), 1))))
		return E_FAIL;

	/*PressPlayBtnUI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_PressPlayBtnUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/LogoUI/PressPlayBtn.dds"), 1))))
		return E_FAIL;

	/* ExitUI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_ExitUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/LogoUI/realgo.dds"), 1))))
		return E_FAIL;

	/* LoadingUI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_LoadingUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/LoadingUI/LoadingUI.dds"), 1))))
		return E_FAIL;


	/* PlayUI - EyeUI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_EyeUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/PlayUI/EyeOff.dds"), 1))))
		return E_FAIL;



	/* PlayUI - HeartUI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HeartUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/PlayUI/heart.dds"), 1))))
		return E_FAIL;
	return S_OK;
}


CMainApp * Client::CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);	

	CGameInstance::Release_Engine();
}

