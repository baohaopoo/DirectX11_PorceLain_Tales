#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "Shader.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
{
	for (auto& pMeshContainer : m_MeshContainers)
		Safe_AddRef(pMeshContainer);	

	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_AddRef(pMaterial.pMaterials[i]);
		}
	}
}

HRESULT CModel::NativeConstruct_Prototype(TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	char			szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);	

	m_eType = eType;

	_uint		iFlag;

	if (TYPE_NONANIM == m_eType)	
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;	

	m_pScene = m_Importer.ReadFile(szFullPath, iFlag);
	if (nullptr == m_pScene)
		return E_FAIL;

	/*m_pScene->mRootNode->mChildren[]*/

	/*m_pScene->mNumAnimations;
	m_pScene->mAnimations[i];*/

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	if (FAILED(Ready_MeshContainers()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshContainerIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;
	
	if (nullptr != m_MeshContainers[iMeshContainerIndex])
		m_MeshContainers[iMeshContainerIndex]->Render();	

	return S_OK;
}

HRESULT CModel::Bind_Material_OnShader(CShader * pShader, aiTextureType eType, const char * pConstantName, _uint iMeshContainerIndex)
{
	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_NumMaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pMaterials[eType]->SetUp_ShaderResourceView(pShader, pConstantName, 0);	
}

HRESULT CModel::Ready_MeshContainers()
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_iNumMeshContainers = m_pScene->mNumMeshes;

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		/* ���������� �о ������ �޽������̳� ��ü�� �ش� ������ �����Բ� ����ܤ�. */
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, m_eType, m_pScene->mMeshes[i], m_PivotMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_iNumMaterials = m_pScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial*			pAIMaterial = m_pScene->mMaterials[i];
		if (nullptr == pAIMaterial)
			return E_FAIL;

		MODELMATERIAL		Materials;
		ZeroMemory(&Materials, sizeof(MODELMATERIAL));		

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char		szTextureFilePath[MAX_PATH] = "";

			aiString	strPath;		
			
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			char		szFileName[MAX_PATH] = "";
			char		szEXT[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			strcpy_s(szTextureFilePath, pModelFilePath);
			strcat_s(szTextureFilePath, szFileName);
			strcat_s(szTextureFilePath, szEXT);

			_tchar		szFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szTextureFilePath, strlen(szTextureFilePath), szFullPath, MAX_PATH);

			Materials.pMaterials[j] = CTexture::Create(m_pDevice, m_pDeviceContext, szFullPath);
			if (nullptr == Materials.pMaterials[j])
				return E_FAIL;				
		}
		m_Materials.push_back(Materials);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	//���� ��������� �׳� ������
	if (nullptr == m_pScene)
		return E_FAIL;


	//
	m_iNumAnimations = m_pScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation* pAIAnimation = m_pScene->mAnimations[i];
		CAnimation* pAnimation = CAnimation::Create(pAIAnimation);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);

	}
	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	CModel*	pInstance = new CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pModelFilePath, pModelFileName, PivotMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = new CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);

	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_Release(pMaterial.pMaterials[i]);
		}		
	}
	m_Materials.clear();

	m_Importer.FreeScene();
}
