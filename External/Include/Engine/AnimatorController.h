#pragma once

#include "Resource.h"
#include "AnimationState.h"

namespace ff7r
{
	class AnimatorController : public Resource
	{
	public:
		AnimatorController();
		~AnimatorController();

		CLONE(AnimatorController);

		AnimationState* GetAnimState(wstring _key);
		Parameter* GetParameter(wstring _key);

		int CreateAnimatorController();

		virtual int Save(const wstring& _relative_path) override;

	private:
		virtual int Load(const wstring& _file_path) override;

		map<wstring, AnimationState*>       anim_states;
		map<wstring, Parameter*>            parameters;

		friend class Animator3D;
	};
}

