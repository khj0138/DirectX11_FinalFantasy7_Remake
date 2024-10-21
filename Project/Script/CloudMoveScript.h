#pragma once

#include <Engine\Script.h>
#include "PlayerDataScript.h"
#include "CloudWeaponScript.h"

namespace ff7r
{
#define INPUT_MOVE_KEY \
	KEY_INPUT(KEY::W) \
	|| KEY_INPUT(KEY::A)\
	|| KEY_INPUT(KEY::D) \
	|| KEY_INPUT(KEY::S)

	class CloudMoveScript : public PlayerDataScript
	{
	public:
		CloudMoveScript();
		~CloudMoveScript();

		CLONE(CloudMoveScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		virtual void Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos = vec3(0.0f, 0.0f, 0.0f)) override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;

		//void ReciveCommandData(CommandData* _data);

		void SetPlayerCamera(GameObject* _camera) { player_camera = _camera; }
		void SetWeapon(CloudWeaponScript* _weapon);
		void RotateCloud(float _origin, float _rot_y);
		void RotateCloudToTarget();

		float GetPlayerCameraRotY();
		bool IsPlayerAtkState();

	private:
		enum class CLOUD_STATE
		{
			IDLE,
			MOVE_ENTER,
			MOVE,
			ATTACK,
			HIT,
			AVOID,
			ABILITY,
			MAGIC,
			LIMIT,
			AI_ATTACK,
			AI_MOVE_ENTER,
			AI_MOVE,
			ITEM,
			END
		};

		void RegisterCharacterAbility(wstring _ability_name);
		void ExecuteCommand();

		void RotateY();
		void ChangeState(CLOUD_STATE _next_state);
		void ChangeIdleState() { ChangeState(CLOUD_STATE::IDLE); }
		void ChangeMoveState();
		void MoveTurnEnd();
		void SetTimeSpeedSlow();
		void SetTimeSpeedOrigin();
		void CheckAIStateChange();
		void ChangeCinematicCameraLimit0();
		void IncreaseCrossSlashEffect();
		void OnBraverSlashEffect();
		void OffBraverSlashEffect();
		void OnWeaponCollider();
		void OffWeaponCollider();
		void OnWeaponCounterEffect() { weapon->PlayCounterStanceEffect(true); }
		void OffWeaponCounterEffect() { weapon->PlayCounterStanceEffect(false); }
		void ShootMagic();
		void BladeBurst();

		void RegisterAnimationEvents();
		void RegisterSkillData();
		void CreateChild();

		vector<class CloudState*> states;

		CloudWeaponScript*		weapon;

		GameObject*				player_camera;
		GameObject*				cross_slash_effect;
		GameObject*				braver_slash_effect;

		class Animator3D*		animator;
		class StateMachine*		state_machine;

		

		CLOUD_STATE				state;
		wstring					magic_name;

		float					rot_y_start;
		float					rot_y_end;
		float					rot_y_time;
		float					time_speed;

		int						n_attack_idx;
		int						rot_y_idx;
		int						limit0_cam_idx;
		int						cross_slash_cnt;
		int						hit_anim_idx;
		int						hit_dir_idx;

		bool					is_rot_y;

		friend class CloudIdleState;
		friend class CloudMoveEnterState;
		friend class CloudMoveState;
		friend class CloudAttackState;
		friend class CloudHitState;
		friend class CloudAvoidState;
		friend class CloudAbilityState;
		friend class CloudMagicState;
		friend class CloudLimitState;
		friend class CloudItemState;
		friend class CloudAIAttackState;
		friend class CloudAIMoveEnterState;
		friend class CloudAIMoveState;
	};
}

