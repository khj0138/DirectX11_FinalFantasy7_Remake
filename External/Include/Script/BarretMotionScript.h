#pragma once

#include <Engine\Script.h>
#include "PlayerDataScript.h"

namespace ff7r
{
#define INPUT_MOVE_KEY \
	KEY_INPUT(KEY::W) \
	|| KEY_INPUT(KEY::A)\
	|| KEY_INPUT(KEY::D) \
	|| KEY_INPUT(KEY::S)

	class CharacterDataScript;
	class BarretWeaponScript;
	class BarretMotionScript : public PlayerDataScript
	{
	public:
		BarretMotionScript();
		~BarretMotionScript();

		CLONE(BarretMotionScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos = vec3(0.0f, 0.0f, 0.0f)) override;


		//void ReciveCommandData(CommandData* _data);

		void SetPlayerCamera(GameObject* _camera) { player_camera = _camera; }
		void RotateBarret(float _origin, float _rot_y);
		void LookTarget(bool _now = false);
		void AttackEnd();
		void CreateEffect();
		void Reload();


		float GetPlayerCameraRotY();
		GameObject* GetTarget() { return target; }
		void SetTarget(GameObject* _target) { target = _target; }

		void SetWeapon(BarretWeaponScript* _weapon);

		void SetTest(bool _test) { is_test = _test; }
		bool GetTest() { return is_test; }
		bool IsPlayerAtkState();



	private:
		enum class BARRET_STATE
		{
			IDLE,
			MOVE_ENTER,
			MOVE,
			ATTACK_ENTER,
			ATTACK,
			AI_MOVE_ENTER,
			AI_MOVE,
			AI_ATTACK_ENTER,
			AI_ATTACK,
			AVOID,
			ABILITY,
			HIT,
			MAGIC,
			LIMIT,
			ITEM,
			END
		};

		enum class EFFECT
		{
			NORMAL_HIT,
			MUZZLE,
			END
		};

		void RotateY();
		void ChangeState(BARRET_STATE _next_state);
		void ChangeIdleState() { ChangeState(BARRET_STATE::IDLE); }
		void ChangeMoveState();
		void ChangeAttackState();
		void LimitCharge();
		void WeaponAttack();
		void WeaponLimit();
		void UseBullet();
		void ChangeCinematicCameraLimit0();
		void SoundOn(int _idx);
		void SoundOff(int _idx);

		void OnEffectScript();
		void OffEffectScript();

		void CheckStateChange();
		void CheckAIStateChange();
		bool CheckChangeKey(KEY _key);
		void MoveTurnEnd();
		void ShootMagic();

		//void ChangeCinematicCameraLimit0();
		//void OnBraverSlashEffect();
		//void OffBraverSlashEffect();

		vector<class BarretState*> states;
		vector<UINT64> state_changes;

		CharacterDataScript* character_data;
		BarretWeaponScript* weapon;

		GameObject* player_camera;

		class Animator3D* animator;
		class StateMachine* state_machine;

		BARRET_STATE state;

		float	rot_y_start;
		float	rot_y_end;
		float	rot_y_time;

		int		n_attack_idx;
		int		rot_y_idx;
		int		limit0_cam_idx;
		int		normal_bullet;
		int		hit_anim_idx;
		int		hit_dir_idx;

		bool	is_rot_y;
		bool	is_test;

		wstring magic_name;
		vector<int>		sound_buf;


		friend class BarretIdleState;
		friend class BarretMoveEnterState;
		friend class BarretMoveState;
		friend class BarretAttackState;
		friend class BarretAttackEnterState;
		friend class BarretAvoidState;
		friend class BarretOverChargeState;
		friend class BarretAbilityState;
		friend class BarretMagicState;
		friend class BarretLimitState;

		friend class BarretAIMoveEnterState;
		friend class BarretAIMoveState;
		friend class BarretAIAttackState;
		friend class BarretAIAttackEnterState;
		friend class BarretHitState;
		friend class BarretItemState;
	};
}