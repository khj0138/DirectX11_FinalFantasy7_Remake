#include "pch.h"
#include "LevelSaveLoad.h"

#include <Engine\PathMgr.h>
#include <Engine\LevelMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>
#include <Engine\GameObject.h>
#include <Engine\components.h>
#include <Engine\Script.h>

#include <Script\ScriptMgr.h>

namespace ff7r
{
	Level* LevelSaveLoad::keep_level = nullptr;

	int LevelSaveLoad::SaveLevel(const wstring& _LevelPath, Level* _Level, bool _is_overwrite)
	{
		if (_Level->GetState() != LEVEL_STATE::STOP)
			return E_FAIL;

		wstring strPath = PathMgr::GetInst()->GetContentPath();
		strPath += _LevelPath;

		FILE* pFile = nullptr;

		if (!_is_overwrite)
		{
			if (_wfopen_s(&pFile, strPath.c_str(), L"r"))
			{
				fclose(pFile);
				return E_FAIL;
			}
		}
		_wfopen_s(&pFile, strPath.c_str(), L"wb");

		if (nullptr == pFile)
			return E_FAIL;

		// 레벨 이름 저장
		SaveWString(_Level->GetName(), pFile);


		// 레벨의 레이어들을 저장
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			Layer* pLayer = _Level->GetLayer(i);

			// 레이어 이름 저장
			SaveWString(pLayer->GetName(), pFile);

			if (pLayer->GetName() != L"Tile" && pLayer->GetName() != L"Capture"
				/*&& pLayer->GetName() != L"Skybox"*/ && pLayer->GetName() != L"Wall")
			{
				continue;
			}

			// 레이어의 게임오브젝트들 저장
			const vector<GameObject*>& vecParent = pLayer->GetParentObject();

			// 오브젝트 개수 저장
			size_t objCount = vecParent.size();
			fwrite(&objCount, sizeof(size_t), 1, pFile);

			// 각 게임오브젝트
			for (size_t i = 0; i < objCount; ++i)
			{
				SaveGameObject(vecParent[i], pFile);
			}
		}

		fclose(pFile);


		return S_OK;
	}

	int LevelSaveLoad::SaveGameObject(GameObject* _Object, FILE* _File)
	{
		// 이름
		SaveWString(_Object->GetName(), _File);

		// 컴포넌트
		for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (i == (UINT)COMPONENT_TYPE::END)
			{
				// 컴포넌트 타입 저장
				fwrite(&i, sizeof(UINT), 1, _File);
				break;
			}

			Component* Com = _Object->GetComponent((COMPONENT_TYPE)i);
			if (nullptr == Com)
				continue;

			// 컴포넌트 타입 저장
			fwrite(&i, sizeof(UINT), 1, _File);

			// 컴포넌트 정보 저장
			Com->SaveToLevelFile(_File);
		}

		// 스크립트	
		const vector<Script*>& vecScript = _Object->GetScripts();
		size_t ScriptCount = vecScript.size();
		fwrite(&ScriptCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			wstring ScriptName = ScriptMgr::GetScriptName(vecScript[i]);
			SaveWString(ScriptName, _File);
			vecScript[i]->SaveToLevelFile(_File);
		}


		// 자식 오브젝트
		const vector<GameObject*>& vecChild = _Object->GetChild();
		size_t ChildCount = vecChild.size();
		fwrite(&ChildCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < ChildCount; ++i)
		{
			SaveGameObject(vecChild[i], _File);
		}

		return 0;
	}

	Level* LevelSaveLoad::LoadLevel(const wstring& _LevelPath, Level* _lv)
	{
		wstring strPath = PathMgr::GetInst()->GetContentPath();
		strPath += _LevelPath;

		FILE* pFile = nullptr;

		_wfopen_s(&pFile, strPath.c_str(), L"rb");

		if (nullptr == pFile)
			return nullptr;

		Level* NewLevel;
		if (_lv)
		{
			NewLevel = _lv;
		}
		else
		{
			NewLevel = new Level;
		}
		
		NewLevel->ChangeState(LEVEL_STATE::PAUSE);
		//NewLevel->ChangeState(LEVEL_STATE::STOP);
		// 레벨 이름
		wstring strLevelName;
		LoadWString(strLevelName, pFile);
		NewLevel->SetName(strLevelName);

		

		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			Layer* pLayer = NewLevel->GetLayer(i);

			// 레이어 이름
			wstring LayerName;
			LoadWString(LayerName, pFile);
			
			pLayer->SetName(LayerName);

			if (pLayer->GetName() != L"Tile" && pLayer->GetName() != L"Capture"
				/*&& pLayer->GetName() != L"Skybox"*/ && pLayer->GetName() != L"Wall")
			{
				continue;
			}

			// 게임 오브젝트 개수
			size_t objCount = 0;
			fread(&objCount, sizeof(size_t), 1, pFile);

			// 각 게임오브젝트
			for (size_t j = 0; j < objCount; ++j)
			{
				GameObject* pNewObj = LoadGameObject(pFile);
				if (i == 1)
				{
					if(pNewObj->GetRenderComponent())
					{
						pNewObj->GetRenderComponent()->SetFrustumCheck(true);
					}
				}
				NewLevel->AddGameObject(pNewObj, i, false);
			}
		}

		fclose(pFile);

		
		
		return NewLevel;
	}

	GameObject* LevelSaveLoad::LoadGameObject(FILE* _File)
	{
		GameObject* pObject = new GameObject;

		// 이름
		wstring Name;
		LoadWString(Name, _File);
		pObject->SetName(Name);

		// 컴포넌트
		while (true)
		{
			UINT ComponentType = 0;
			fread(&ComponentType, sizeof(UINT), 1, _File);

			// 컴포넌트 정보의 끝을 확인
			if ((UINT)COMPONENT_TYPE::END == ComponentType)
				break;

			Component* Component = nullptr;

			switch ((COMPONENT_TYPE)ComponentType)
			{
			case COMPONENT_TYPE::TRANSFORM:
				Component = new Transform;
				break;
			case COMPONENT_TYPE::COLLIDER2D:
				Component = new Collider2D;
				break;
			case COMPONENT_TYPE::COLLIDER3D:
				Component = new Collider3D;
				break;
			case COMPONENT_TYPE::ANIMATOR2D:
				Component = new Animator2D;
				break;
			case COMPONENT_TYPE::ANIMATOR3D:
				Component = new Animator3D;
				break;
			case COMPONENT_TYPE::LIGHT2D:
				Component = new Light2D;
				break;
			case COMPONENT_TYPE::LIGHT3D:
				Component = new Light3D;
				break;
			case COMPONENT_TYPE::CAMERA:
				Component = new Camera;
				break;
			case COMPONENT_TYPE::REFLECTIONCAPTURE:
				Component = new ReflectionCapture;
				break;
			case COMPONENT_TYPE::SOCKET:
				Component = new Socket;
				break;
			case COMPONENT_TYPE::MESHRENDER:
				Component = new MeshRender;
				break;
			case COMPONENT_TYPE::PARTICLESYSTEM:
				Component = new ParticleSystem;
				break;
			case COMPONENT_TYPE::TILEMAP:
				Component = new TileMap;
				break;
			case COMPONENT_TYPE::LANDSCAPE:
				break;
			case COMPONENT_TYPE::SKYBOX:
				Component = new SkyBox;
				break;
			case COMPONENT_TYPE::DECAL:
				break;
			}
			if (Component == nullptr)
			{
				continue;
			}
			Component->LoadFromLevelFile(_File);
			pObject->AddComponent(Component);
		}


		// 스크립트	
		size_t ScriptCount = 0;
		fread(&ScriptCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < ScriptCount; ++i)
		{
			wstring ScriptName;
			LoadWString(ScriptName, _File);
			Script* pScript = ScriptMgr::GetScript(ScriptName);
			pObject->AddComponent(pScript);
			pScript->LoadFromLevelFile(_File);
		}

		// 자식 오브젝트		
		size_t ChildCount = 0;
		fread(&ChildCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < ChildCount; ++i)
		{
			GameObject* ChildObject = LoadGameObject(_File);
			pObject->AddChild(ChildObject);
		}

		return pObject;
	}

	Level* LevelSaveLoad::GetKeepLevel()
	{
		if (keep_level)
		{
			Level* temp = keep_level;
			keep_level = nullptr;
			return temp;
		}
		return nullptr;
	}


	void LevelSaveLoad::LoadLevelKeep(const wstring& _LevelPath, Level* _lv)
	{
		if (keep_level)
		{
			return;
		}

		keep_level = LoadLevel(_LevelPath, _lv);
	}
}