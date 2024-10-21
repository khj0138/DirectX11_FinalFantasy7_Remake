#pragma once

#include "CharacterDataScript.h"
#include "SoldierWeaponScript.h"

namespace ff7r
{
	class SoldierScript : public CharacterDataScript
	{
	public:
		SoldierScript();
		~SoldierScript();

		CLONE(SoldierScript);

		virtual void Awake() override;
		virtual void Tick() override;
		virtual void Start() override;

		virtual void Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos = vec3(0.0f, 0.0f, 0.0f)) override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;

		void SetWeapon(SoldierWeaponScript* _weapon);
		void RegisterDeadEvent(function<void()> _event) { dead_event = _event; }

	private:
		SoldierWeaponScript* weapon;
		class Animator3D* animator;

		float time;

		bool is_ground;
		bool is_dead;

		function<void()> dead_event;

		friend class SoldierAttackState;
		friend class SoldierIdleState;
		friend class SoldierHitState;
	};
}

