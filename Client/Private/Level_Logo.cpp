#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"


CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	

	if (FAILED(Ready_Layer_ExitBtnUI(TEXT("Layer_PlayBtn"))))
		return E_FAIL;




	return S_OK;
}

void CLevel_Logo::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	//ExitBtn 누르면, 모양달라지고, 몇초후 fade in.out인 된다.
	RECT rc{ 100,0,200,300 };
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&rc, pt)) {

		//모양만 바꾸기



		//우클릭을 아예 해버린다면, 
		if (GetAsyncKeyState(VK_LBUTTON)) {
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			if (FAILED(Ready_Layer_MenuUI(TEXT("Layer_MenuUI"))))
				return;

			RELEASE_INSTANCE(CGameInstance);
		}

	}


	RECT Exitpos{ 100,350,200,650 };
	POINT pt2;
	GetCursorPos(&pt2);
	ScreenToClient(g_hWnd, &pt2);

	if (PtInRect(&Exitpos, pt2)) {

		//모양만 바꾸기

		//우클릭을 아예 해버린다면, 
		if (GetAsyncKeyState(VK_LBUTTON)) {
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			if (FAILED(Ready_Layer_ExitUI(TEXT("Layer_ExitUI"))))
				return;

			RELEASE_INSTANCE(CGameInstance);
		}

	}


	//F1키를 누르면 다음 장면으로 넘어간다.
	if (GetKeyState(VK_F1) & 0x8000)
	{
		if (FAILED(Ready_Layer_MenuUI(TEXT("Layer_MenuUI"))))
			return;
	}
	// 기존에 누르던 키를 누르고 있으
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_GAMEPLAY))))
			return;


	}

	Safe_Release(pGameInstance);

}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_MenuUI(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_MenuUI"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_ExitUI(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_ExitUI"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_ExitBtnUI(const _tchar * pLayerTag)
{
	//사실 여긴 PlayBtn임
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_ExitBtnUI"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Background(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_LogoMap"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
