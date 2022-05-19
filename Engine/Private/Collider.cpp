#include "..\Public\Collider.h"
#include "PipeLine.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
#ifdef _DEBUG
	, m_pBasicEffect(rhs.m_pBasicEffect)
	, m_pBatch(rhs.m_pBatch)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
	Safe_AddRef(m_pInputLayout);
}

HRESULT CCollider::NativeConstruct_Prototype(TYPE eType)
{
	m_eType = eType;

	m_pBasicEffect = new BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	COLLIDERDESC		ColliderDesc;

	if (nullptr != pArg)
		memcpy(&ColliderDesc, pArg, sizeof(COLLIDERDESC));

	else
	{
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
		ColliderDesc.fRadius = 1.f;
	}	
	
	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
		m_pAABB = new BoundingBox(ColliderDesc.vPosition, _float3(ColliderDesc.vSize.x * 0.5f, ColliderDesc.vSize.y * 0.5f, ColliderDesc.vSize.z * 0.5f));
		m_pAABBInWorld = new BoundingBox();
		break;
	case CCollider::TYPE_OBB:
		m_pOBB = new BoundingOrientedBox(ColliderDesc.vPosition, _float3(ColliderDesc.vSize.x * 0.5f, ColliderDesc.vSize.y * 0.5f, ColliderDesc.vSize.z * 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case CCollider::TYPE_SPHERE:
		m_pSPHERE = new BoundingSphere(ColliderDesc.vPosition, ColliderDesc.fRadius);
		break;
	}

	return S_OK;
}

void CCollider::Update(_fmatrix WorldMatrix)
{


	m_pAABB->Transform(*m_pAABBInWorld, WorldMatrix);

}

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	_matrix		ViewMatrix, ProjMatrix;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	ViewMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ProjMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	m_pBasicEffect->SetWorld(XMMatrixIdentity());
	m_pBasicEffect->SetView(ViewMatrix);
	m_pBasicEffect->SetProjection(ProjMatrix);

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pBasicEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABBInWorld);
		break;
	case CCollider::TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBB);
		break;
	case CCollider::TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSPHERE);
		break;
	}

	m_pBatch->End();	

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}
#endif // _DEBUG

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType)
{
	CCollider *	pInstance = new CCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSG_BOX(TEXT("Failed to Created CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider *	pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG

	Safe_Release(m_pInputLayout);

	if (false == m_isCloned)
	{		
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pBasicEffect);
	}

	
#endif // _DEBUG
}
