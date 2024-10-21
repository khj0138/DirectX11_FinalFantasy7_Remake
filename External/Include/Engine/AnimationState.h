#pragma once

#include "Animation3D.h"
#include "ptr.h"
namespace ff7r
{
	union Data
	{
		Data(float _f)	{ f = _f; }
		Data(int _i)	{ i = _i; }
		Data(bool _b)	{ b = _b; }
		Data(): i(0) {}
		
		void operator = (float _f) { f = _f; }
		void operator = (int _i) { i = _i; }
		void operator = (bool _b) { b = _b; }

		float f;
		int i;
		bool b;
	};

	enum class DATA_TYPE
	{
		INT,
		FLOAT,
		BOOL,
		TRIGGER
	};
	
	struct Parameter
	{
		Data data = {};
		DATA_TYPE type = DATA_TYPE::INT;

		wstring& GetKey() { return key; }
		void SetKey(wstring _key) { key = _key; }
		
	private:
		wstring key;
	};

	enum class COMPARISON
	{
		LESS,
		GREATER,
		EQUALS,
		NOTEQUAL,
		C_TRUE,
		C_FALSE
	};
	

	struct Condition
	{
		Parameter* input_data;
		Data compare_data;

		COMPARISON comparsion;
	};

	struct Transition
	{
		class AnimationState* next_state;
		vector<Condition> conditions;

		int check_frame_idx = -1;
		int start_frame_idx = 0;

		bool always_chek;

		vector<Condition>& GetConditions() { return conditions; }
	};

	class Animator3D;
	class AnimationState : public Entity
	{
	public:
		AnimationState();
		~AnimationState();

		CLONE(AnimationState);

		void FinalTick();
		AnimationState* CheckTransition();
		void UpdateData();
		void EnterState(float _wait_time, int _start_idx);

		void Reset() { animation->Reset(); }
		void Clear() { animation->Clear(); }

		bool IsWait() { return is_wait; }

		void AddCondition(Transition* _transition, wstring _param_key, Data _compare, COMPARISON _comparison);
		void SetTransition(Transition* _transition) { transitions.push_back(_transition); }
		vector<Transition*>& GetTransitions() { return transitions; }
		void SetAnimation(Ptr<Animation3D> _anim) { animation = _anim; }
		Animation3D* GetAnimation() { return animation.Get(); }
		void SetAnimator(Animator3D* _owner) { animator = _owner; }
		void SetNodePosition(vec2 _pos) { node_pos = _pos; }
		

		Animator3D* GetAnimator() { return animator; }
		vec2 GetNodePosition() { return node_pos; }

		void Save(FILE* _file);
		void Load(FILE* _file, class AnimatorController* _controller);

		float				wait_time;
	private:
		bool CheckCondition(Condition& condition);

		vector<Transition*>	transitions;
		Ptr<Animation3D>	animation;
		Animator3D*			animator;

		vec2				node_pos;


		bool				is_wait;
	};
}

