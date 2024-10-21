#include "pch.h"

#include "CloudMoveScript.h"
#include "BarretMotionScript.h"

#include "AtkRangeScript.h"
namespace ff7r
{
	AtkRangeScript::AtkRangeScript()
		: Script(SCRIPT_TYPE::ATKRANGESCRIPT)
		, player(nullptr)
		, overlap_cnt(0)
	{
	}

	AtkRangeScript::~AtkRangeScript()
	{
	}

	void AtkRangeScript::Awake()
	{
		GetCollider3D()->SetTrigger(true);
		GetCollider3D()->SetDynamic(false);
		GetCollider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		GetCollider3D()->SetOffsetPos(vec3(0.4f, 0.9f, 0.0f));
		GetCollider3D()->SetOffsetScale(vec3(1.0f, 1.8f, 1.0f));
	}

	void AtkRangeScript::Tick()
	{
		if (overlap_cnt > 0)
		{
			for (int i = 0; i < collider.size(); i++)
			{
				if (collider[i])
				{
					OnOverlap(collider[i], vector{ vec3(0.f, 0.f, 0.f) });
				}
			}
		}
		else
		{
			player->GetAnimator3D()->SetMoveFront(true);
		}
	}

	void AtkRangeScript::BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		//collider.resize(overlap_cnt + 1);
		//collider[overlap_cnt] = _other;
		overlap_cnt++;

		for (int i=0; i< collider.size(); i++)
		{
			if (collider[i] == nullptr)
			{
				collider[i] = _other;
				return;
			}
		}
		collider.push_back(_other);

		//animator->test = true;
	}

	void AtkRangeScript::OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		bool _is_player_atk = false;
		CloudMoveScript* _cloud = player->GetScript<CloudMoveScript>();
		if (_cloud != nullptr)
		{
			_is_player_atk = _cloud->IsPlayerAtkState();
		}
		else
		{
			BarretMotionScript* _barret = player->GetScript<BarretMotionScript>();
			if (_barret != nullptr)
			{
				_is_player_atk = _barret->IsPlayerAtkState();
			}
		}

		if (_is_player_atk)
		{
			player->GetAnimator3D()->SetMoveFront(false);
		}
		else
		{
			player->GetAnimator3D()->SetMoveFront(true);
		}
	}

	void AtkRangeScript::EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos)
	{
		overlap_cnt--;
		for (int i = 0; i < collider.size(); i++)
		{
			if (collider[i] == _other)
			{
				collider[i] = nullptr;
				return;
			}
		}
		
		if (overlap_cnt <= 0)
		{
			player->GetAnimator3D()->SetMoveFront(true);
		}
	}

}