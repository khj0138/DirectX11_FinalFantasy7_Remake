#include "pch.h"

#include "PathMgr.h"

#include "AnimatorController.h"
namespace ff7r
{
	AnimatorController::AnimatorController()
		: Resource(RES_TYPE::ANIMATOR_CONTROLLER)
	{
		
	}

	AnimatorController::~AnimatorController()
	{
		for (auto iter = anim_states.begin(); iter != anim_states.end(); iter++)
		{
			if (iter->second)
			{
				delete iter->second;
				iter->second = nullptr;
			}
		}

		for (auto iter = parameters.begin(); iter != parameters.end(); iter++)
		{
			if (iter->second)
			{
				delete iter->second;
				iter->second = nullptr;
			}
		}
	}

	AnimationState* AnimatorController::GetAnimState(wstring _key)
	{
		auto iter = anim_states.find(_key);

		if (iter != anim_states.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	Parameter* AnimatorController::GetParameter(wstring _key)
	{
		auto iter = parameters.find(_key);

		if (iter != parameters.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	int AnimatorController::CreateAnimatorController()
	{
		if (!anim_states.empty())
		{
			return E_FAIL;
		}

		AnimationState* _default_state = new AnimationState();
		_default_state->SetName(L"default");
		anim_states.insert({ L"default", _default_state });

		AnimationState* _any_state = new AnimationState();
		_any_state->SetName(L"any_state");
		anim_states.insert({ L"any_state", _any_state });

		return S_OK;
	}

	int AnimatorController::Save(const wstring& _relative_path)
	{
		wstring _file_path = PathMgr::GetInst()->GetContentPath() + _relative_path;

		path dirPath = path(_file_path).parent_path();
		create_directories(dirPath);

		FILE* _file = nullptr;
		errno_t err = _wfopen_s(&_file, _file_path.c_str(), L"wb");
		assert(_file);

		// 키값, 상대 경로	
		SaveWString(GetName(), _file);
		SaveWString(GetKey(), _file);

		int cnt = parameters.size();
		fwrite(&cnt, sizeof(int), 1, _file);

		for (auto iter = parameters.begin(); iter != parameters.end(); iter++)
		{
			SaveWString(iter->first, _file);
			fwrite(&iter->second->data, sizeof(Data), 1, _file);
			fwrite(&iter->second->type, sizeof(DATA_TYPE), 1, _file);
		}

		cnt = anim_states.size();
		fwrite(&cnt, sizeof(int), 1, _file);

		for (auto iter = anim_states.begin(); iter != anim_states.end(); iter++)
		{
			SaveWString(iter->first, _file);
		}

		for (auto iter = anim_states.begin(); iter != anim_states.end(); iter++)
		{
			iter->second->Save(_file);
		}

		fclose(_file);
		return S_OK;
	}
	int AnimatorController::Load(const wstring& _file_path)
	{
		FILE* _file = nullptr;
		errno_t err = _wfopen_s(&_file, _file_path.c_str(), L"rb");
		assert(_file);

		wstring strName, strKey, strRelativePath;
		LoadWString(strName, _file);
		SetName(strName);

		LoadWString(strKey, _file);

		int cnt;
		fread(&cnt, sizeof(int), 1, _file);

		for (int i = 0; i < cnt; i++)
		{
			wstring _key;
			LoadWString(_key, _file);

			Parameter* _param = new Parameter;

			fread(&_param->data, sizeof(Data), 1, _file);
			fread(&_param->type, sizeof(DATA_TYPE), 1, _file);
			_param->SetKey(_key);

			parameters.insert({ _key, _param });
		}

		fread(&cnt, sizeof(int), 1, _file);

		for (int i = 0; i < cnt; i++)
		{
			AnimationState* _state = new AnimationState;
			// _state->SetAnimator(this);

			wstring _state_name;
			LoadWString(_state_name, _file);
			_state->SetName(_state_name);

			anim_states.insert({ _state_name, _state });
		}

		for (auto iter = anim_states.begin(); iter != anim_states.end(); iter++)
		{
			iter->second->Load(_file, this);
		}

		fclose(_file);

		return S_OK;
	}
}