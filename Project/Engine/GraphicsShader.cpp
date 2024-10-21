#include "pch.h"
#include "GraphicsShader.h"

#include "PathMgr.h"
#include "Device.h"

namespace ff7r
{
	GraphicsShader::GraphicsShader()
		: Shader(RES_TYPE::GRAPHICS_SHADER)
		, topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, rs_type(RS_TYPE::CULL_BACK)
		, ds_type(DS_TYPE::LESS)
		, bs_type(BS_TYPE::DEFAULT)
		, domain(SHADER_DOMAIN::DOMAIN_UNDEFINED)
	{
	}

	GraphicsShader::~GraphicsShader()
	{
	}

	void GraphicsShader::CreateVertexShader(const wstring& _strFileName, const string& _strFuncName)
	{
		// Shader 파일 경로
		wstring strShaderFile = PathMgr::GetInst()->GetContentPath();
		strShaderFile += _strFileName;

		// VertexShader Compile
		if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _strFuncName.c_str(), "vs_5_0", 0, 0, vs_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Vertex Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		DEVICE->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize()
			, nullptr, vs.GetAddressOf());


		// InputLayout 생성
		D3D11_INPUT_ELEMENT_DESC LayoutDesc[8] = {};

		LayoutDesc[0].SemanticName = "POSITION";
		LayoutDesc[0].SemanticIndex = 0;
		LayoutDesc[0].InputSlot = 0;
		LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[0].InstanceDataStepRate = 0;
		LayoutDesc[0].AlignedByteOffset = 0;
		LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;


		LayoutDesc[1].SemanticName = "COLOR";
		LayoutDesc[1].SemanticIndex = 0;
		LayoutDesc[1].InputSlot = 0;
		LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[1].InstanceDataStepRate = 0;
		LayoutDesc[1].AlignedByteOffset = 12;
		LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		LayoutDesc[2].SemanticName = "TEXCOORD";
		LayoutDesc[2].SemanticIndex = 0;
		LayoutDesc[2].InputSlot = 0;
		LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[2].InstanceDataStepRate = 0;
		LayoutDesc[2].AlignedByteOffset = 28;
		LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;

		LayoutDesc[3].SemanticName = "NORMAL";
		LayoutDesc[3].SemanticIndex = 0;
		LayoutDesc[3].InputSlot = 0;
		LayoutDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[3].InstanceDataStepRate = 0;
		LayoutDesc[3].AlignedByteOffset = 36;
		LayoutDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[4].SemanticName = "TANGENT";
		LayoutDesc[4].SemanticIndex = 0;
		LayoutDesc[4].InputSlot = 0;
		LayoutDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[4].InstanceDataStepRate = 0;
		LayoutDesc[4].AlignedByteOffset = 48;
		LayoutDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[5].SemanticName = "BINORMAL";
		LayoutDesc[5].SemanticIndex = 0;
		LayoutDesc[5].InputSlot = 0;
		LayoutDesc[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[5].InstanceDataStepRate = 0;
		LayoutDesc[5].AlignedByteOffset = 60;
		LayoutDesc[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[6].SemanticName = "BLENDWEIGHT";
		LayoutDesc[6].SemanticIndex = 0;
		LayoutDesc[6].AlignedByteOffset = 72;
		LayoutDesc[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[6].InputSlot = 0;
		LayoutDesc[6].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[6].InstanceDataStepRate = 0;

		LayoutDesc[7].SemanticName = "BLENDINDICES";
		LayoutDesc[7].SemanticIndex = 0;
		LayoutDesc[7].AlignedByteOffset = 88;
		LayoutDesc[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[7].InputSlot = 0;
		LayoutDesc[7].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[7].InstanceDataStepRate = 0;

		if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 8
			, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize()
			, input_layout.GetAddressOf())))
		{
			assert(nullptr);
		}
	}

	void GraphicsShader::CreateInstVertexShader(const wstring& _strFileName, const string& _strFuncName)
	{
		wstring strShaderFile = PathMgr::GetInst()->GetContentPath();
		strShaderFile += _strFileName;

		// VertexShader Compile
		if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _strFuncName.c_str(), "vs_5_0", 0, 0, vs_inst_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Vertex Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		DEVICE->CreateVertexShader(vs_inst_blob->GetBufferPointer(), vs_inst_blob->GetBufferSize()
			, nullptr, vs_inst.GetAddressOf());

		// InputLayout 생성
		D3D11_INPUT_ELEMENT_DESC LayoutDesc[21] = {};

		LayoutDesc[0].SemanticName = "POSITION";
		LayoutDesc[0].SemanticIndex = 0;
		LayoutDesc[0].InputSlot = 0;
		LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[0].InstanceDataStepRate = 0;
		LayoutDesc[0].AlignedByteOffset = 0;
		LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;


		LayoutDesc[1].SemanticName = "COLOR";
		LayoutDesc[1].SemanticIndex = 0;
		LayoutDesc[1].InputSlot = 0;
		LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[1].InstanceDataStepRate = 0;
		LayoutDesc[1].AlignedByteOffset = 12;
		LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		LayoutDesc[2].SemanticName = "TEXCOORD";
		LayoutDesc[2].SemanticIndex = 0;
		LayoutDesc[2].InputSlot = 0;
		LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[2].InstanceDataStepRate = 0;
		LayoutDesc[2].AlignedByteOffset = 28;
		LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;

		LayoutDesc[3].SemanticName = "NORMAL";
		LayoutDesc[3].SemanticIndex = 0;
		LayoutDesc[3].InputSlot = 0;
		LayoutDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[3].InstanceDataStepRate = 0;
		LayoutDesc[3].AlignedByteOffset = 36;
		LayoutDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[4].SemanticName = "TANGENT";
		LayoutDesc[4].SemanticIndex = 0;
		LayoutDesc[4].InputSlot = 0;
		LayoutDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[4].InstanceDataStepRate = 0;
		LayoutDesc[4].AlignedByteOffset = 48;
		LayoutDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[5].SemanticName = "BINORMAL";
		LayoutDesc[5].SemanticIndex = 0;
		LayoutDesc[5].InputSlot = 0;
		LayoutDesc[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[5].InstanceDataStepRate = 0;
		LayoutDesc[5].AlignedByteOffset = 60;
		LayoutDesc[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[6].SemanticName = "BLENDWEIGHT";
		LayoutDesc[6].SemanticIndex = 0;
		LayoutDesc[6].AlignedByteOffset = 72;
		LayoutDesc[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[6].InputSlot = 0;
		LayoutDesc[6].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[6].InstanceDataStepRate = 0;

		LayoutDesc[7].SemanticName = "BLENDINDICES";
		LayoutDesc[7].SemanticIndex = 0;
		LayoutDesc[7].AlignedByteOffset = 88;
		LayoutDesc[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[7].InputSlot = 0;
		LayoutDesc[7].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[7].InstanceDataStepRate = 0;

		LayoutDesc[8].SemanticName = "WORLD";
		LayoutDesc[8].SemanticIndex = 0;
		LayoutDesc[8].AlignedByteOffset = 0;
		LayoutDesc[8].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[8].InputSlot = 1;
		LayoutDesc[8].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[8].InstanceDataStepRate = 1;

		LayoutDesc[9].SemanticName = "WORLD";
		LayoutDesc[9].SemanticIndex = 1;
		LayoutDesc[9].AlignedByteOffset = 16;
		LayoutDesc[9].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[9].InputSlot = 1;
		LayoutDesc[9].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[9].InstanceDataStepRate = 1;

		LayoutDesc[10].SemanticName = "WORLD";
		LayoutDesc[10].SemanticIndex = 2;
		LayoutDesc[10].AlignedByteOffset = 32;
		LayoutDesc[10].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[10].InputSlot = 1;
		LayoutDesc[10].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[10].InstanceDataStepRate = 1;

		LayoutDesc[11].SemanticName = "WORLD";
		LayoutDesc[11].SemanticIndex = 3;
		LayoutDesc[11].AlignedByteOffset = 48;
		LayoutDesc[11].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[11].InputSlot = 1;
		LayoutDesc[11].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[11].InstanceDataStepRate = 1;

		LayoutDesc[12].SemanticName = "WV";
		LayoutDesc[12].SemanticIndex = 0;
		LayoutDesc[12].AlignedByteOffset = 64;
		LayoutDesc[12].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[12].InputSlot = 1;
		LayoutDesc[12].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[12].InstanceDataStepRate = 1;

		LayoutDesc[13].SemanticName = "WV";
		LayoutDesc[13].SemanticIndex = 1;
		LayoutDesc[13].AlignedByteOffset = 80;
		LayoutDesc[13].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[13].InputSlot = 1;
		LayoutDesc[13].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[13].InstanceDataStepRate = 1;

		LayoutDesc[14].SemanticName = "WV";
		LayoutDesc[14].SemanticIndex = 2;
		LayoutDesc[14].AlignedByteOffset = 96;
		LayoutDesc[14].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[14].InputSlot = 1;
		LayoutDesc[14].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[14].InstanceDataStepRate = 1;

		LayoutDesc[15].SemanticName = "WV";
		LayoutDesc[15].SemanticIndex = 3;
		LayoutDesc[15].AlignedByteOffset = 112;
		LayoutDesc[15].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[15].InputSlot = 1;
		LayoutDesc[15].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[15].InstanceDataStepRate = 1;

		LayoutDesc[16].SemanticName = "WVP";
		LayoutDesc[16].SemanticIndex = 0;
		LayoutDesc[16].AlignedByteOffset = 128;
		LayoutDesc[16].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[16].InputSlot = 1;
		LayoutDesc[16].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[16].InstanceDataStepRate = 1;

		LayoutDesc[17].SemanticName = "WVP";
		LayoutDesc[17].SemanticIndex = 1;
		LayoutDesc[17].AlignedByteOffset = 144;
		LayoutDesc[17].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[17].InputSlot = 1;
		LayoutDesc[17].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[17].InstanceDataStepRate = 1;

		LayoutDesc[18].SemanticName = "WVP";
		LayoutDesc[18].SemanticIndex = 2;
		LayoutDesc[18].AlignedByteOffset = 160;
		LayoutDesc[18].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[18].InputSlot = 1;
		LayoutDesc[18].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[18].InstanceDataStepRate = 1;

		LayoutDesc[19].SemanticName = "WVP";
		LayoutDesc[19].SemanticIndex = 3;
		LayoutDesc[19].AlignedByteOffset = 176;
		LayoutDesc[19].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[19].InputSlot = 1;
		LayoutDesc[19].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[19].InstanceDataStepRate = 1;

		LayoutDesc[20].SemanticName = "ROWINDEX";
		LayoutDesc[20].SemanticIndex = 0;
		LayoutDesc[20].AlignedByteOffset = 192;
		LayoutDesc[20].Format = DXGI_FORMAT_R32_UINT;
		LayoutDesc[20].InputSlot = 1;
		LayoutDesc[20].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc[20].InstanceDataStepRate = 1;

		if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 21
			, vs_inst_blob->GetBufferPointer(), vs_inst_blob->GetBufferSize()
			, input_layout_inst.GetAddressOf())))
		{
			assert(nullptr);
		}
	}

	void GraphicsShader::CreateHullShader(const wstring& _strFileName, const string& _strFuncName)
	{
		// Shader 파일 경로
		wstring strShaderFile = PathMgr::GetInst()->GetContentPath();
		strShaderFile += _strFileName;

		// Shader Compile	
		if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _strFuncName.c_str(), "hs_5_0", 0, 0, hs_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 Shader 를 만든다.
		DEVICE->CreateHullShader(hs_blob->GetBufferPointer(), hs_blob->GetBufferSize()
			, nullptr, hs.GetAddressOf());
	}

	void GraphicsShader::CreateDomainShader(const wstring& _strFileName, const string& _strFuncName)
	{
		// Shader 파일 경로
		wstring strShaderFile = PathMgr::GetInst()->GetContentPath();
		strShaderFile += _strFileName;

		// Shader Compile	
		if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _strFuncName.c_str(), "ds_5_0", 0, 0, ds_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 Shader 를 만든다.
		DEVICE->CreateDomainShader(ds_blob->GetBufferPointer(), ds_blob->GetBufferSize()
			, nullptr, ds.GetAddressOf());
	}

	void GraphicsShader::CreateGeometryShader(const wstring& _strFileName, const string& _strFuncName)
	{
		// Shader 파일 경로
		wstring strShaderFile = PathMgr::GetInst()->GetContentPath();
		strShaderFile += _strFileName;

		// Shader Compile	
		if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _strFuncName.c_str(), "gs_5_0", 0, 0, gs_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 Shader 를 만든다.
		DEVICE->CreateGeometryShader(gs_blob->GetBufferPointer(), gs_blob->GetBufferSize()
			, nullptr, gs.GetAddressOf());
	}

	void GraphicsShader::CreatePixelShader(const wstring& _strFileName, const string& _strFuncName)
	{
		// Shader 파일 경로
		wstring strShaderFile = PathMgr::GetInst()->GetContentPath();
		strShaderFile += _strFileName;


		// PixelShader Compile	
		if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _strFuncName.c_str(), "ps_5_0", 0, 0, ps_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Pixel Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 PixelShader 를 만든다.
		DEVICE->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize()
			, nullptr, ps.GetAddressOf());
	}

	void GraphicsShader::UpdateData()
	{
		CONTEXT->IASetInputLayout(input_layout.Get());
		CONTEXT->IASetPrimitiveTopology(topology);

		CONTEXT->VSSetShader(vs.Get(), nullptr, 0);
		CONTEXT->HSSetShader(hs.Get(), nullptr, 0);
		CONTEXT->DSSetShader(ds.Get(), nullptr, 0);
		CONTEXT->GSSetShader(gs.Get(), nullptr, 0);
		CONTEXT->PSSetShader(ps.Get(), nullptr, 0);

		CONTEXT->RSSetState(Device::GetInst()->GetRSState(rs_type).Get());
		CONTEXT->OMSetDepthStencilState(Device::GetInst()->GetDSState(ds_type).Get(), 0);
		CONTEXT->OMSetBlendState(Device::GetInst()->GetBSState(bs_type).Get(), vec4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	}

	void GraphicsShader::UpdateDataInst()
	{
		CONTEXT->IASetInputLayout(input_layout_inst.Get());
		CONTEXT->IASetPrimitiveTopology(topology);

		CONTEXT->VSSetShader(vs_inst.Get(), nullptr, 0);
		CONTEXT->HSSetShader(hs.Get(), nullptr, 0);
		CONTEXT->DSSetShader(ds.Get(), nullptr, 0);
		CONTEXT->GSSetShader(gs.Get(), nullptr, 0);
		CONTEXT->PSSetShader(ps.Get(), nullptr, 0);

		CONTEXT->RSSetState(Device::GetInst()->GetRSState(rs_type).Get());
		CONTEXT->OMSetDepthStencilState(Device::GetInst()->GetDSState(ds_type).Get(), 10);
		CONTEXT->OMSetBlendState(Device::GetInst()->GetBSState(bs_type).Get(), vec4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	}
}