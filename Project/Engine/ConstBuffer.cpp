#include "pch.h"
#include "ConstBuffer.h"

#include "Device.h"
namespace ff7r
{
	ConstBuffer::ConstBuffer(UINT _register_num)
		: desc{}
		, register_number(_register_num)
		, element_size(0)
		, element_count(0)
	{
	}

	ConstBuffer::~ConstBuffer()
	{
	}

	void ConstBuffer::Create(UINT _element_size, UINT _element_count)
	{
		element_size = _element_size;
		element_count = _element_count;

		UINT buffer_size = _element_size * _element_count;

		// 16����Ʈ ���� �޸� ����
		assert(!(buffer_size % 16));

		// ������� ����
		desc.ByteWidth = buffer_size;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, const_buff.GetAddressOf())))
		{
			assert(nullptr);
		}
	}

	void ConstBuffer::SetData(void* _src, UINT _size)
	{
		// ũ�Ⱑ �������� ���� �����ʹ� ������� ũ��� ����.
		UINT size = _size;
		if (0 == _size)
		{
			size = element_size * element_count;
		}

		// ������� ũ�⺸�� �� ū �����Ͱ� �Է����� ���� ���
		assert(!(size > element_size * element_count));

		// SysMem -> GPU Mem
		D3D11_MAPPED_SUBRESOURCE sub_res = {};
		if (!FAILED(CONTEXT->Map(const_buff.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &sub_res)))
		{
			memcpy(sub_res.pData, _src, size);
			CONTEXT->Unmap(const_buff.Get(), 0);
		}
	}

	void ConstBuffer::UpdateData()
	{
		CONTEXT->VSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
		CONTEXT->HSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
		CONTEXT->DSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
		CONTEXT->GSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
		CONTEXT->PSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
	}

	void ConstBuffer::UpdateData_CS()
	{
		CONTEXT->CSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
	}
}