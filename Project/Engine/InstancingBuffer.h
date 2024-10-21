#pragma once

#include "StructuredBuffer.h"
#include "Texture.h"
#include "CopyBoneShader.h"
#include "ptr.h"

namespace ff7r
{
	class InstancingBuffer : public Singleton<InstancingBuffer>
	{
	public:
		//InstancingBuffer();
		//~InstancingBuffer();

		void Init();
		void Clear()
		{
			m_vecData.clear();
			m_vecBoneMat.clear();
			m_iAnimInstCount = 0;
		}
		void AddInstancingData(InstancingData& _tData) { m_vecData.push_back(_tData); }

		UINT GetInstanceCount() { return (UINT)m_vecData.size(); }
		ComPtr<ID3D11Buffer> GetBuffer() { return m_pInstancingBuffer; }
		void SetData();

		void AddInstancingBoneMat(StructuredBuffer* _pBuffer);
		int GetAnimInstancingCount() { return m_iAnimInstCount; };
		StructuredBuffer* GetBoneBuffer() { return m_pBoneBuffer; }

	private:
		SINGLE(InstancingBuffer);
		void Resize(UINT _iCount);
		ComPtr<ID3D11Buffer>		m_pInstancingBuffer;
		UINT						m_iMaxCount;
		vector<InstancingData>		m_vecData;

		// anim3D ¿ëµµ
		vector<StructuredBuffer*>	m_vecBoneMat;
		int							m_iAnimInstCount;
		StructuredBuffer* m_pBoneBuffer;

		Ptr<CopyBoneShader>		m_pCopyShader;

	};
}
