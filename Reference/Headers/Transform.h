#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_double			SpeedPerSec;
		_double			RotationPerSec;

	}TRANSFORMDESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);	
	virtual ~CTransform() = default;

public: // Getter 
	_vector Get_State(STATE eState) {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
	}

	_float3 Get_Scale();

	_matrix Get_WorldMatrix();
	_matrix Get_WorldMatrixInverse();
	_float4x4 Get_WorldFloat4x4();
	_float4x4 Get_WorldFloat4x4_TP();


public: // Setter 
	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);
	}

public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg = nullptr);

public:
	HRESULT Bind_WorldMatrixOnShader(class CShader* pShader, const char* pConstantName);

public:
	HRESULT Go_Straight(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT Go_BackWard(_double TimeDelta);

	HRESULT Scaled(const _float3& vScale);
	HRESULT Turn(_fvector vAxis, _double TimeDelta);
	HRESULT Rotation(_fvector vAxis, _double Radian);


private:	
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END