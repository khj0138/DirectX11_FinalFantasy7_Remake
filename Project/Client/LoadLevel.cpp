#include "pch.h"

#include <Engine\LevelMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>
#include <Engine\GameObject.h>
#include <Engine\components.h>
#include <Engine\ResourceMgr.h>
#include <Engine\RenderMgr.h>
#include <Engine\EventMgr.h>
#include <Engine\Device.h>
#include <Engine\KeyMgr.h>

#include <Script\UIMgr.h>
#include <Script\ImageUIScript.h>
#include <Script\TextUIScript.h>
#include "EditorObjMgr.h"
#include <Script\func.h>
#include "LevelSaveLoad.h"
#include <Engine\ThreadPool.h>
#include <Script\GameMgr.h>

#include "LoadLevel.h"
namespace ff7r
{
	LoadLevel::LoadLevel()
		: complete_load(false)
		, is_first_tick(true)
		, next_level(nullptr)
		, register_level(false)
		, load_character(false)
		, b(true)
		, temp(nullptr)
	{
	}

	LoadLevel::~LoadLevel()
	{
	}

	void LoadLevel::Tick()
	{
		if (!complete_load)
		{
			if (is_first_tick)
			{
				ChangeState(LEVEL_STATE::STOP);
				Init();
				Refresh();
				wstring strContentPath = wstring(PathMgr::GetInst()->GetContentPath());
				FindFileName(strContentPath, L"Stage1");
				Load();
				//ResourceMgr::GetInst()->ResourceThreadWait();
				is_first_tick = false;
			}
			else
			{
				//if (path.size() > 0)
				//{
				//	Load();
				//	ResourceMgr::GetInst()->ResourceThreadWait();
				//	Device::GetInst()->ExcuteCommadList();
				//}
				//else
				{

					//Device::GetInst()->ExcuteCommadList();
					if (ResourceMgr::GetInst()->ResourceThreadEmpty())
					{
						ResourceMgr::GetInst()->ResourceThreadWait();
						//Device::GetInst()->ExcuteCommadList();

						complete_load = true;

						//LevelSaveLoad::LoadLevelKeep(L"Level\\8.5 14.56.lv", temp);
						LevelSaveLoad::LoadLevelKeep(L"Level\\9.1 20.42.lv", temp);
					}
				}
			}
		}
		else if (!next_level)
		{
			next_level = LevelSaveLoad::GetKeepLevel();
		}


		if (!register_level)
		{
			if (next_level)
			{
				register_level = true;
				LevelMgr::GetInst()->CreateLevel(next_level);
				load_character = true;
				//ThreadPool thread(1);
				//
				//thread.Enqueue([this]() {Create(); });
			}
		}
		else
		{
			if (b && load_character)
			{
				if (KEY_TAP(KEY::ENTER))
				{
					b = false;


					Event evn = {};
					evn.type = EVENT_TYPE::LEVEL_CHANGE;
					evn.w_param = (DWORD_PTR)next_level;
					Device::GetInst()->ExcuteCommadList();
					EventMgr::GetInst()->AddEvent(evn);
					GameMgr::GetInst()->ChangeLevel(GameMgr::LEVEL_TYPE::STAGE2);
				}
			}
		}

		Level::Tick();
	}

	void LoadLevel::FinalTick()
	{
		Level::FinalTick();
	}

	void LoadLevel::Refresh()
	{
		temp = new Level();
		temp->ChangeState(LEVEL_STATE::PAUSE);
		LevelMgr::GetInst()->ChangeLevel(LevelMgr::GetInst()->GetLevel(0), temp);
		ResourceMgr::GetInst()->Test();
	}

	void LoadLevel::Init()
	{
		GameObject* center = new GameObject;
		center->SetName(L"center");
		center->AddComponent(new Transform);
		AddGameObject(center, L"Default");

		for(int i=0; i<8; i++)
		{
			GameObject* _load = new GameObject;
			_load->SetName(L"Title");
			_load->AddComponent(new Transform);
			_load->AddComponent(new MeshRender);
			_load->AddComponent(new ImageUIScript);

			_load->GetTransform()->SetRelativePosXY(vec2(0.0f, 0.0f));
			_load->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
			_load->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
			_load->Awake();
			_load->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\UIAtlas01.png"));
			_load->SetName(L"Load UI");
			_load->SetLayerIndex(0);
			UIScript* _ui = _load->GetScript<UIScript>();
			_ui->SetSize(vec2(34.f, 34.f), vec2(1.0f, 1.0f));
			_ui->SetLeftTop(vec2(347.f, 1120.f));

			center->AddChild(_load);
		}
		// 347 1120 34 34
	}

	void LoadLevel::FindFileName(const wstring& _FolderPath, wstring exclude)
	{
		WIN32_FIND_DATA FindData = {};
		wstring FolderPath = _FolderPath + L"*.*";
		HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

		if (hFindHandle == INVALID_HANDLE_VALUE)
		{
			return;
		}

		do
		{
			if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !wcscmp(FindData.cFileName, exclude.c_str()))
			{
				continue;
			}

			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (!wcscmp(FindData.cFileName, L".") || !wcscmp(FindData.cFileName, L"..")) {
					continue;
				}

				// 하위 폴더 탐색
				FindFileName(_FolderPath + FindData.cFileName + L"\\", exclude);
				continue;
			}

			wstring strContentPath = PathMgr::GetInst()->GetContentPath();
			wstring RelativePath = _FolderPath + FindData.cFileName;
			RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

			path.push_back(RelativePath);

		} while (FindNextFile(hFindHandle, &FindData));

		FindClose(hFindHandle);
	}

	RES_TYPE LoadLevel::GetResTypeByExt(const wstring& _relativepath)
	{
		wchar_t szExt[50] = {};
		_wsplitpath_s(_relativepath.c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);
		wstring strExt = szExt;

		if (L".mdat" == strExt)
			return RES_TYPE::MESHDATA;
		else if (L".pref" == strExt)
			return RES_TYPE::PREFAB;
		else if (L".mesh" == strExt)
			return RES_TYPE::MESH;
		else if (L".mtrl" == strExt)
			return RES_TYPE::MATERIAL;
		else if (L".png" == strExt || L".jpg" == strExt
			|| L".jpeg" == strExt || L".bmp" == strExt
			|| L".tga" == strExt || L".dds" == strExt
			|| L".hdr" == strExt)
			return RES_TYPE::TEXTURE;
		else if (L".mp3" == strExt || L".wav" == strExt || L".oga" == strExt)
			return RES_TYPE::SOUND;
		else if (L".anim" == strExt)
			return RES_TYPE::ANIMATION3D;
		else if (L".animctrl" == strExt)
			return RES_TYPE::ANIMATOR_CONTROLLER;
		else if (L".pref" == strExt)
			return RES_TYPE::PREFAB;
		else
			return RES_TYPE::END;
	}

	void LoadLevel::Load()
	{
		int i = 0;
		for (auto _path = path.begin(); _path != path.end();)
		{
			RES_TYPE type = GetResTypeByExt(*_path);

			if (type == RES_TYPE::END)
			{
				_path = path.erase(_path);
				continue;
			}
			i++;
			switch (type)
			{
			case RES_TYPE::MESHDATA:
				ResourceMgr::GetInst()->MultiThreadLoad<MeshData>(*_path, *_path);
				break;
			case RES_TYPE::MATERIAL:
				ResourceMgr::GetInst()->MultiThreadLoad<Material>(*_path, *_path);
				break;
			case RES_TYPE::PREFAB:
				ResourceMgr::GetInst()->MultiThreadLoad<Prefab>(*_path, *_path);
				break;
			case RES_TYPE::MESH:
				ResourceMgr::GetInst()->MultiThreadLoad<Mesh>(*_path, *_path);
				break;
			case RES_TYPE::TEXTURE:
				ResourceMgr::GetInst()->MultiThreadLoad<Texture>(*_path, *_path);
				break;
			case RES_TYPE::SOUND:
				ResourceMgr::GetInst()->MultiThreadLoad<Sound>(*_path, *_path);
				break;
			case RES_TYPE::ANIMATION3D:
				ResourceMgr::GetInst()->MultiThreadLoad<Animation3D>(*_path, *_path);
				break;
			case RES_TYPE::ANIMATOR_CONTROLLER:
				ResourceMgr::GetInst()->MultiThreadLoad<AnimatorController>(*_path, *_path);
				break;
			default: break;
			}

			_path = path.erase(_path);

			/*	if (i > 1000)
					break;*/
		}
	}

	void CreateLoadLevel()
	{
		Level* level = new LoadLevel();
		Event evn = {};
		evn.type = EVENT_TYPE::LEVLE_CHANGE_ANOTHER;
		evn.w_param = (DWORD_PTR)level;

		EventMgr::GetInst()->AddEvent(evn);
	}


	void RegisterLoadLevelFunc()
	{
		SetLoadLevelFunc([]() { CreateLoadLevel(); });
	}
}