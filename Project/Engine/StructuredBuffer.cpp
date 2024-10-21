#include "pch.h"
#include "StructuredBuffer.h"

#include "Device.h"

namespace ff7r
{
	StructuredBuffer::StructuredBuffer()
		: element_size(0)
		, element_count(0)
		, register_number(0)
		, desc{}
		, system_access(false)
		, type(SB_TYPE::READ_ONLY)
	{
	}

	StructuredBuffer::~StructuredBuffer()
	{
	}

	void StructuredBuffer::Create(UINT _element_size, UINT _element_count, SB_TYPE _type
		, bool _use_sys_access, void* _sys_mem)
	{
		structed_buff = nullptr;
		srv = nullptr;
		uav = nullptr;

		read_buff = nullptr;
		write_buff = nullptr;

		type = _type;
		system_access = _use_sys_access;

		element_size = _element_size;
		element_count = _element_count;

		UINT buff_size = element_size * element_count;

		// 16����Ʈ ���� �޸� ����

		// ============ �ӽ÷� ���� =========
		 // assert(!(buff_size % 16));
		// ============ �ӽ÷� ���� =========


		// ������� ����
		desc.ByteWidth = buff_size;				// ���� ũ��
		desc.StructureByteStride = element_size;	// ������ ����

		if (SB_TYPE::READ_ONLY == type)
		{
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// Texture �������Ϳ� ���̵��ϱ� ���� �÷���
		}
		else if (SB_TYPE::READ_WRITE == type)
		{
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		}

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// ����ȭ ���� üũ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;

		if (nullptr == _sys_mem)
		{
			if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, structed_buff.GetAddressOf())))
			{
				assert(nullptr);
			}
			// structed_buff->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"structed buff") - 1, L"structed buff");
		}
		else
		{
			D3D11_SUBRESOURCE_DATA sub_res_data = {};
			sub_res_data.pSysMem = _sys_mem;

			HRESULT hr = DEVICE->CreateBuffer(&desc, &sub_res_data, structed_buff.GetAddressOf());
			if (hr)
			{
				assert(nullptr);
			}
		}

		// ShaderResourceView ����
		D3D11_SHADER_RESOURCE_VIEW_DESC	_shader_res_desc = {};

		_shader_res_desc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
		_shader_res_desc.BufferEx.NumElements = element_count;

		if (FAILED(DEVICE->CreateShaderResourceView(structed_buff.Get(), &_shader_res_desc, srv.GetAddressOf())))
		{
			assert(nullptr);
		}

		if (SB_TYPE::READ_WRITE == type)
		{

			D3D11_UNORDERED_ACCESS_VIEW_DESC unordered_access_desc = {};
			unordered_access_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			unordered_access_desc.Buffer.NumElements = element_count;

			if (FAILED(DEVICE->CreateUnorderedAccessView(structed_buff.Get(), &unordered_access_desc, uav.GetAddressOf())))
			{
				assert(nullptr);
			}
		}

		// CPU Access ���� ����
		if (system_access)
		{
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

			// GPU -> CPU Read
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, read_buff.GetAddressOf())))
			{
				assert(nullptr);
			}

			// CPU -> GPU Write
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, write_buff.GetAddressOf())))
			{
				assert(nullptr);
			}
		}
	}

	void StructuredBuffer::SetData(void* _src, UINT _size)
	{
		if (nullptr == _src)
			return;

		UINT iSize = _size;
		if (0 == iSize)
		{
			iSize = GetBufferSize();
		}

		// CPU -> CPU WriteBuffer
		D3D11_MAPPED_SUBRESOURCE _sub = {};
		CONTEXT->Map(write_buff.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_sub);
		memcpy(_sub.pData, _src, iSize);
		CONTEXT->Unmap(write_buff.Get(), 0);

		// CPU WriteBuffer -> Main Buffer
		CONTEXT->CopyResource(structed_buff.Get(), write_buff.Get());
	}

	void StructuredBuffer::GetData(void* _dst)
	{
		// Main Buffer -> CPU ReadBuffer
		CONTEXT->CopyResource(read_buff.Get(), structed_buff.Get());

		// CPU ReadBuffer -> CPU
		D3D11_MAPPED_SUBRESOURCE tSub = {};
		CONTEXT->Map(read_buff.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &tSub);
		memcpy(_dst, tSub.pData, GetBufferSize());
		CONTEXT->Unmap(read_buff.Get(), 0);
	}


	void StructuredBuffer::UpdateData(UINT _register_num, UINT _stage)
	{
		register_number = _register_num;

		if (PIPELINE_STAGE::PS_VERTEX & _stage)
		{
			CONTEXT->VSSetShaderResources(_register_num, 1, srv.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_HULL & _stage)
		{
			CONTEXT->HSSetShaderResources(_register_num, 1, srv.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_DOMAIN & _stage)
		{
			CONTEXT->DSSetShaderResources(_register_num, 1, srv.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_GEOMETRY & _stage)
		{
			CONTEXT->GSSetShaderResources(_register_num, 1, srv.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_PIXEL & _stage)
		{
			CONTEXT->PSSetShaderResources(_register_num, 1, srv.GetAddressOf());
		}
	}

	void StructuredBuffer::UpdateDataCS(UINT _register_num, bool _is_shader_res)
	{
		register_number = _register_num;

		if (_is_shader_res)
		{
			CONTEXT->CSSetShaderResources(_register_num, 1, srv.GetAddressOf());
		}
		else
		{
			UINT i = -1;
			CONTEXT->CSSetUnorderedAccessViews(_register_num, 1, uav.GetAddressOf(), &i);
		}
	}

	void StructuredBuffer::Clear()
	{
		ID3D11ShaderResourceView* _srv = nullptr;
		CONTEXT->VSSetShaderResources(register_number, 1, &_srv);
		CONTEXT->HSSetShaderResources(register_number, 1, &_srv);
		CONTEXT->DSSetShaderResources(register_number, 1, &_srv);
		CONTEXT->GSSetShaderResources(register_number, 1, &_srv);
		CONTEXT->PSSetShaderResources(register_number, 1, &_srv);
		CONTEXT->CSSetShaderResources(register_number, 1, &_srv);

	}

	void StructuredBuffer::ClearCS(bool _is_shader_res)
	{
		if (_is_shader_res)
		{
			ID3D11ShaderResourceView* _srv = nullptr;
			CONTEXT->CSSetShaderResources(register_number, 1, &_srv);
		}
		else
		{
			ID3D11UnorderedAccessView* _uav = nullptr;
			UINT i = -1;
			CONTEXT->CSSetUnorderedAccessViews(register_number, 1, &_uav, &i);
		}
	}
}