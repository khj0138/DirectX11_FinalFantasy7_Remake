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
#include <Script\GameMgr.h>

#include "EditorObjMgr.h"

#include "TestLevel.h"
#include "LevelSaveLoad.h"
#include "TitleLevel.h"

#include <Engine\ThreadPool.h>

namespace ff7r
{
	TitleLevel::TitleLevel()
		: complete_load(false)
		, is_first_tick(true)
		, next_level(nullptr)
		, register_level(false)
		, load_character(false)
		, b(true)
	{
	}
	TitleLevel::~TitleLevel()
	{
	}

	void TitleLevel::Tick()
	{
		if (!complete_load)
		{
			if (is_first_tick)
			{
				ChangeState(LEVEL_STATE::STOP);
				Init();
				wstring strContentPath = wstring(PathMgr::GetInst()->GetContentPath()) + L"Stage1\\";
				FindFileName(strContentPath, L"Stage2");
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
						RenderMgr::GetInst()->SetAmbientIntensity(0.3f);
						complete_load = true;
						//LevelSaveLoad::LoadLevelKeep(L"Level\\Stage1 4.4 20.40.lv");
						LevelSaveLoad::LoadLevelKeep(L"Level\\Stage1 9.2 7.37.lv");
					}
				}
			}
		}
		else if (!next_level)
		{
			next_level = LevelSaveLoad::GetKeepLevel();
		}


		if(!register_level)
		{
			if (next_level)
			{
				register_level = true;
				LevelMgr::GetInst()->CreateLevel(next_level);

				ThreadPool thread(1);

				thread.Enqueue([this]() {Create(); });
			}
		}
		else
		{
			if (b && load_character)
			{
				button->SetActive(true);
				if (KEY_TAP(KEY::ENTER))
				{
					b = false;

					Event evn = {};
					evn.type = EVENT_TYPE::LEVEL_CHANGE;
					evn.w_param = (DWORD_PTR)next_level;
					Device::GetInst()->ExcuteCommadList();
					EventMgr::GetInst()->AddEvent(evn);
					GameMgr::GetInst()->ChangeLevel(GameMgr::LEVEL_TYPE::STAGE1);
				}
			}
		}



		Level::Tick();
	}

	void TitleLevel::FinalTick()
	{
		Level::FinalTick();
	}

	void TitleLevel::FindFileName(const wstring& _FolderPath, wstring exclude)
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

	RES_TYPE TitleLevel::GetResTypeByExt(const wstring& _relativepath)
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

	void TitleLevel::Init()
	{		
		//CreateChapter17HundredGunnerBossStage();
		ChangeState(LEVEL_STATE::STOP);

		GameObject* camera = new GameObject;
		camera->SetName(L"MainCamera");
		camera->AddComponent(new Transform);
		camera->AddComponent(new Camera);
		camera->GetCamera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		camera->GetCamera()->SetCameraIndex(0);
		camera->GetCamera()->SetLayerMaskAll(true);

		AddGameObject(camera, L"Default");
		RenderMgr::GetInst()->RegisterEditorCamera(camera->GetCamera());

		GameObject* title = new GameObject;
		title->SetName(L"Title");
		title->AddComponent(new Transform);
		title->AddComponent(new MeshRender);
		title->AddComponent(new ImageUIScript);

		title->GetTransform()->SetRelativePosXY(vec2(0.0f, 111.0f));
		title->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		title->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		title->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\U_StartMenu_00.png"));
		title->SetName(L"Character2 HUD");

		UIScript* _ui = title->GetScript<UIScript>();
		_ui->SetSize(vec2(980.f, 576.f), vec2(1.0f, 1.0f));
		_ui->SetLeftTop(vec2(0.f,0.f));

		AddGameObject(title, L"Default");


		button = new GameObject;
		button->AddComponent(new Transform);
		button->AddComponent(new FontRenderer);
		button->AddComponent(new TextUIScript);
	
		button->GetTransform()->SetRelativePosXY(-137.f, -314.f);
		button->GetTransform()->SetRelativeScaleXY(0.6f, 0.6f);
		button->SetLayerIndex(31);
		button->SetName(L"PRESS ANY BUTTON");

		TextUIScript* _text_ui = button->GetScript<TextUIScript>();
		_text_ui->CreateDynamicMtrl();
		_text_ui->SetText("PRESS ANY BUTTON");

		
		// 682 92 626 853

		title = new GameObject;
		title->SetName(L"Title");
		title->AddComponent(new Transform);
		title->AddComponent(new MeshRender);
		title->AddComponent(new ImageUIScript);

		title->GetTransform()->SetRelativePosXY(vec2(162.f, 0.f));
		title->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
		title->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"UIMtrl"));
		title->GetMeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\U_StartMenu_00.png"));
		title->SetName(L"Character2 HUD");

		ImageUIScript* _image_ui = title->GetScript<ImageUIScript>();
		_image_ui->SetSize(vec2(682.f, 92.f), vec2(1.0f, 1.0f));
		_image_ui->SetLeftTop(vec2(626.f, 853.f));
		_image_ui->SetTimeSpeed(1.0f);
		_image_ui->SetAlphaFactor(1.5f);
		_image_ui->SetImageEffectType(ImageUIScript::ImageEffectType::ALPHA_CHANGE);
		button->AddChild(title);

		AddGameObject(button, L"Default");
		button->SetActive(false);
	}

	void TitleLevel::Load()
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

	void TitleLevel::Create()
	{
		CreateTestLevel();
		UIMgr::GetInst()->Init();
		load_character = true;
	}


	void CreateTitleLevel()
	{
		LevelMgr::GetInst()->ChangeLevel(new TitleLevel);
		GameMgr::GetInst()->ChangeLevel(GameMgr::LEVEL_TYPE::TITLE);
	}
}