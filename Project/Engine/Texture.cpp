#include "pch.h"

#include "Device.h"
#include "PathMgr.h"

#include "ResourceMgr.h"

#include "Texture.h"
namespace ff7r
{
	using std::max;
	Texture::Texture(bool _bEngine)
		: Resource(RES_TYPE::TEXTURE, _bEngine)
		, desc{}
		, register_num(0)
	{
		rtv.push_back(ComPtr<ID3D11RenderTargetView>());
	}

	Texture::~Texture()
	{
	}


	void Texture::UpdateData(int _iRegisterNum, int _PipelineStage)
	{
		register_num = _iRegisterNum;

		if (PIPELINE_STAGE::PS_VERTEX & _PipelineStage)
		{
			CONTEXT->VSSetShaderResources(_iRegisterNum, 1, srv.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_HULL & _PipelineStage)
		{
			CONTEXT->HSSetShaderResources(_iRegisterNum, 1, srv.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_DOMAIN & _PipelineStage)
		{
			CONTEXT->DSSetShaderResources(_iRegisterNum, 1, srv.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_GEOMETRY & _PipelineStage)
		{
			CONTEXT->GSSetShaderResources(_iRegisterNum, 1, srv.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_PIXEL & _PipelineStage)
		{
			CONTEXT->PSSetShaderResources(_iRegisterNum, 1, srv.GetAddressOf());
		}
	}

	void Texture::UpdateDataCS(int _iRegisterNum, bool _bShaderRes)
	{
		register_num = _iRegisterNum;

		if (_bShaderRes)
		{
			CONTEXT->CSSetShaderResources(register_num, 1, srv.GetAddressOf());
		}
		else
		{
			UINT i = -1;
			CONTEXT->CSSetUnorderedAccessViews(register_num, 1, uav.GetAddressOf(), &i);
		}
	}

	void Texture::Clear()
	{
		ID3D11ShaderResourceView* pSRV = nullptr;
		CONTEXT->VSSetShaderResources(register_num, 1, &pSRV);
		CONTEXT->HSSetShaderResources(register_num, 1, &pSRV);
		CONTEXT->DSSetShaderResources(register_num, 1, &pSRV);
		CONTEXT->GSSetShaderResources(register_num, 1, &pSRV);
		CONTEXT->PSSetShaderResources(register_num, 1, &pSRV);

	}

	void Texture::Clear(int _iRegisterNum)
	{
		ID3D11ShaderResourceView* pSRV = nullptr;
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
	}

	void Texture::ClearCS(bool _bShaderRes)
	{
		if (_bShaderRes)
		{
			ID3D11ShaderResourceView* pSRV = nullptr;
			CONTEXT->CSSetShaderResources(register_num, 1, &pSRV);
		}
		else
		{
			ID3D11UnorderedAccessView* pUAV = nullptr;
			UINT i = -1;
			CONTEXT->CSSetUnorderedAccessViews(register_num, 1, &pUAV, &i);
		}
	}

	int Texture::Load(const wstring& _strFilePath)
	{
		wchar_t szExt[100] = L"";
		_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 100);
		wstring strExt = szExt;

		HRESULT hr = S_OK;
		if (L".dds" == strExt || L".DDS" == strExt)
		{
			// dds, DDS
			hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, image);
		}

		else if (L".tga" == strExt || L".TGA" == strExt)
		{
			// tga, TGA
			hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, image);
		}
		else if (L".hdr" == strExt)
		{
			hr = LoadFromHDRFile(_strFilePath.c_str(), nullptr, image);
		}
		else
		{
			// png, jpg, jpeg, bmp
			hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, image);
		}


		desc.Width = image.GetMetadata().width;
		desc.Height = image.GetMetadata().height;
		desc.MipLevels = 0; // 모든 MIP 레벨을 생성하도록 설정
		desc.ArraySize = 1;
		desc.Format = image.GetMetadata().format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; // MIP 맵 생성을 위해 필요
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		hr = DEVICE->CreateTexture2D(&desc, nullptr, &texture2d);
		if (FAILED(hr))
		{
			int a = 0;
		}

		ResourceMgr::GetInst()->GetTextureMutex()->lock();
		RES_CONTEXT->UpdateSubresource(texture2d.Get(), 0, nullptr, image.GetImages()->pixels, image.GetImages()->rowPitch, 0);
		

		// Shader Resource View 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1; // 모든 MIP 레벨 사용

		hr = DEVICE->CreateShaderResourceView(texture2d.Get(), &srvDesc, srv.GetAddressOf());
		if (FAILED(hr))
		{
			MessageBox(nullptr, L"ShaderResourceView 생성 실패", L"텍스처 로딩 실패", MB_OK);
			return E_FAIL;
		}

		// MIP 맵 생성
		RES_CONTEXT->GenerateMips(srv.Get());
		ResourceMgr::GetInst()->GetTextureMutex()->unlock();

		//
		//if (FAILED(hr))
		//{
		//	MessageBox(nullptr, L"리소스 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
		//	return E_FAIL;
		//}
		//
		//hr = CreateShaderResourceView(DEVICE
		//	, image.GetImages()
		//	, image.GetImageCount()
		//	, image.GetMetadata()
		//	, srv.GetAddressOf());
		//
		//if (FAILED(hr))
		//{
		//	MessageBox(nullptr, L"ShaderResourceView 생성 실패", L"텍스쳐 로딩 실패", MB_OK);
		//	return E_FAIL;
		//}
		//
		//srv->GetResource((ID3D11Resource**)texture2d.GetAddressOf());
		//texture2d->GetDesc(&desc);

		return S_OK;
	}

	void Texture::UpdateData()
	{
	}

	int Texture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage)
	{
		// ID3D11Texture2D 생성
		desc.Format = _pixelformat;

		// 반드시 렌더타겟과 같은 해상도로 설정해야 함
		desc.Width = _Width;
		desc.Height = _Height;
		desc.ArraySize = 1;

		desc.BindFlags = _BindFlag;
		desc.Usage = _Usage;

		if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		else if (D3D11_USAGE::D3D11_USAGE_STAGING == _Usage)
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		desc.MipLevels = 1;    // 원본만 생성
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;


		if (FAILED(DEVICE->CreateTexture2D(&desc, nullptr, texture2d.GetAddressOf())))
		{
			return E_FAIL;
		}

		// 바인드 플래그에 맞는 View 를 생성해준다.
		if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			if (FAILED(DEVICE->CreateDepthStencilView(texture2d.Get(), nullptr, dsv.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
		else
		{
			if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
			{
				if (FAILED(DEVICE->CreateRenderTargetView(texture2d.Get(), nullptr, rtv[0].GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
			{
				if (FAILED(DEVICE->CreateShaderResourceView(texture2d.Get(), nullptr, srv.GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
			{
				if (FAILED(DEVICE->CreateUnorderedAccessView(texture2d.Get(), nullptr, uav.GetAddressOf())))
				{
					return E_FAIL;
				}
			}
		}


		return S_OK;
	}

	int Texture::Create(ComPtr<ID3D11Texture2D> _tex2D)
	{
		texture2d = _tex2D;
		
		texture2d->GetDesc(&desc);

		// 바인드 플래그에 맞는 View 를 생성해준다.
		if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			if (FAILED(DEVICE->CreateDepthStencilView(texture2d.Get(), nullptr, dsv.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
		else
		{
			if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
			{
				if (FAILED(DEVICE->CreateRenderTargetView(texture2d.Get(), nullptr, rtv[0].GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
			{
				if (FAILED(DEVICE->CreateShaderResourceView(texture2d.Get(), nullptr, srv.GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
			{
				if (FAILED(DEVICE->CreateUnorderedAccessView(texture2d.Get(), nullptr, uav.GetAddressOf())))
				{
					return E_FAIL;
				}
			}
		}

		return S_OK;
	}

	int Texture::CreateArray(UINT _arr_size)
	{
		desc.Height = 32;
		desc.Width = 32;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.ArraySize = _arr_size;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, texture2d.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
		viewdesc.Format = desc.Format;
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		viewdesc.Texture2DArray.MipLevels = desc.MipLevels;
		viewdesc.Texture2DArray.MostDetailedMip = 0;
		viewdesc.Texture2DArray.ArraySize = _arr_size;

		DEVICE->CreateShaderResourceView(texture2d.Get(), &viewdesc, srv.GetAddressOf());

		rtv.resize(_arr_size);

		D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
		rtv_desc.Format = desc.Format;
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtv_desc.Texture2DArray.ArraySize = 1;
		rtv_desc.Texture2DArray.MipSlice = 0;

		for (UINT i = 0; i < _arr_size; i++)
		{
			rtv_desc.Texture2DArray.FirstArraySlice = i;

			if (FAILED(DEVICE->CreateRenderTargetView(texture2d.Get(), &rtv_desc, rtv[i].GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		//CONTEXT->PSSetShaderResources(15, 1, texture2d.Get()->GetSRV().GetAddressOf());
		
		return true;
	}

	int Texture::CreateCube(D3D11_TEXTURE2D_DESC _desc, bool _use_generate_mip)
	{
		desc = _desc;
		rtv.resize(6);

		if (_use_generate_mip)
		{
			rtv.resize(36);
		}

		if (FAILED(DEVICE->CreateTexture2D(&desc, nullptr, texture2d.GetAddressOf())))
		{
			return E_FAIL;
		}


		if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			if (FAILED(DEVICE->CreateDepthStencilView(texture2d.Get(), nullptr, dsv.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
		else
		{
			if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
			{
				D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
				rtv_desc.Format = desc.Format;
				rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				rtv_desc.Texture2DArray.ArraySize = 1;
				rtv_desc.Texture2DArray.MipSlice = 0;

				if (_use_generate_mip)
				{
					for (int face = 0; face < 6; ++face)
					{
						rtv_desc.Texture2DArray.FirstArraySlice = face;

						for(int i=0; i<6; i++)
						{
							rtv_desc.Texture2DArray.MipSlice = i;
							if (FAILED(DEVICE->CreateRenderTargetView(texture2d.Get(), &rtv_desc, rtv[face *6 + i].GetAddressOf())))
							{
								return E_FAIL;
							}
						}
					}
				}
				else
				{
					for (int face = 0; face < 6; ++face)
					{
						rtv_desc.Texture2DArray.FirstArraySlice = face;
						rtv_desc.Texture2DArray.MipSlice = 0;

						if (FAILED(DEVICE->CreateRenderTargetView(texture2d.Get(), &rtv_desc, rtv[face].GetAddressOf())))
						{
							return E_FAIL;
						}
					}
				}
			}

			if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc = {};
				_srv_desc.Format = desc.Format;
				_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				_srv_desc.TextureCube.MostDetailedMip = 0; // 가장 상세한 밉 레벨
				_srv_desc.TextureCube.MipLevels = -1;

				if (FAILED(DEVICE->CreateShaderResourceView(texture2d.Get(), &_srv_desc, srv.GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
			{
				if (FAILED(DEVICE->CreateUnorderedAccessView(texture2d.Get(), nullptr, uav.GetAddressOf())))
				{
					return E_FAIL;
				}
			}
		}

		return S_OK;
	}

	int Texture::CreateCubeArray(vector<Ptr<Texture>> _cube_array, UINT _cnt, UINT _width, UINT _height, UINT _mip_level, UINT _slot)
	{
		if (texture2d) 
		{
			texture2d.Reset();
		}
		if (srv) 
		{
			srv.Reset();
		}
		for (auto& rtvView : rtv) 
		{
			if (rtvView) 
			{
				rtvView.Reset();
			}
		}

		desc = {};
		desc.Width = _width;  
		desc.Height = _height; 
		desc.MipLevels = _mip_level;
		desc.ArraySize = _cnt * 6;
		desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, texture2d.GetAddressOf());

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
		srvDesc.TextureCubeArray.MostDetailedMip = 0;
		srvDesc.TextureCubeArray.MipLevels = -1;
		srvDesc.TextureCubeArray.First2DArrayFace = 0;
		srvDesc.TextureCubeArray.NumCubes = _cnt;

		if (FAILED(DEVICE->CreateShaderResourceView(texture2d.Get(), &srvDesc, srv.GetAddressOf())))
		{
			return E_FAIL;
		}
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = desc.Format; 
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0; // 사용할 Mip Level
		rtvDesc.Texture2DArray.FirstArraySlice = 0; // 시작 배열 슬라이스 인덱스
		rtvDesc.Texture2DArray.ArraySize = _cnt * 6; // 큐브의 개수 * 각 큐브의 면 수

		// 렌더 타겟 뷰 생성
		DEVICE->CreateRenderTargetView(texture2d.Get(), &rtvDesc, rtv[0].GetAddressOf());



		for (UINT cubeMapIndex = 0; cubeMapIndex < _cnt; ++cubeMapIndex)
		{
			for (UINT faceIndex = 0; faceIndex < 6; ++faceIndex)
			{
				for (UINT i = 0; i < _mip_level; i++)
				{
					UINT srcSubresource = D3D11CalcSubresource(i, faceIndex, 6);
					UINT dstSubresource = D3D11CalcSubresource(i, cubeMapIndex * 6 + faceIndex, 6);

					CONTEXT->CopySubresourceRegion(texture2d.Get(), dstSubresource, 0, 0, 0
						, _cube_array[cubeMapIndex]->texture2d.Get(), srcSubresource, nullptr);
				}
			}
		}

		//CONTEXT->GenerateMips(srv.Get());
		CONTEXT->PSSetShaderResources(_slot, 1, srv.GetAddressOf());

		return S_OK;
	}

	int Texture::Save(const wstring& _strRelativePath)
	{
		return S_OK;

		wstring strFilePath = PathMgr::GetInst()->GetContentPath();
		strFilePath += _strRelativePath;

		// 캡처 (GPU -> Sys)
		CaptureTexture(DEVICE, CONTEXT, texture2d.Get(), image);

		HRESULT hr = S_OK;
		size_t arrsize = image.GetMetadata().arraySize;

		// 밉맵이 1 이상인 경우
		if (1 < desc.MipLevels)
		{
			D3D11_TEXTURE2D_DESC texdesc = {};
			texdesc = desc;
			texdesc.MipLevels = 1;

			ComPtr<ID3D11Texture2D> pCopyTex2D;
			HRESULT hr = DEVICE->CreateTexture2D(&texdesc, nullptr, pCopyTex2D.GetAddressOf());

			for (int i = 0; i < (int)desc.ArraySize; ++i)
			{
				// m_Tex2D ==> pCopyTex2D
				UINT iDestSubresIdx = D3D11CalcSubresource(0, i, 1);
				UINT iSrcSubresIdx = D3D11CalcSubresource(0, i, /*MAX_MIP*/8);

				CONTEXT->CopySubresourceRegion(pCopyTex2D.Get(), iDestSubresIdx, 0, 0, 0
					, texture2d.Get(), iSrcSubresIdx, nullptr);
			}

			ScratchImage tempimage;
			CaptureTexture(DEVICE, CONTEXT, pCopyTex2D.Get(), tempimage);

			if (1 < arrsize)
			{
				hr = SaveToDDSFile(tempimage.GetImages()
					, arrsize, tempimage.GetMetadata()
					, DDS_FLAGS::DDS_FLAGS_NONE
					, strFilePath.c_str());
			}
			else
			{
				hr = SaveToWICFile(*(tempimage.GetImages())
					, WIC_FLAGS_NONE
					, GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG))
					, strFilePath.c_str());
			}
		}

		// 밉맵이 1 레벨(원본만) 있는 경우, 캡쳐 후 바로 저장
		else
		{
			if (1 < arrsize)
			{
				hr = SaveToDDSFile(image.GetImages()
					, arrsize
					, image.GetMetadata()
					, DDS_FLAGS::DDS_FLAGS_NONE
					, strFilePath.c_str());
			}
			else
			{
				hr = SaveToWICFile(*(image.GetImages())
					, WIC_FLAGS_NONE
					, GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG))
					, strFilePath.c_str());
			}
		}

		return hr;
	}
}