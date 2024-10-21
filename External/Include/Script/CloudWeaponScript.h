#pragma once

#include <Engine\Script.h>
#include "AttackDataScript.h"

namespace ff7r
{
	class CloudWeaponScript : public AttackDataScript
	{
	public:
		CloudWeaponScript();
		~CloudWeaponScript();

		CLONE(CloudWeaponScript);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Tick() override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;

		enum class WEAPON_TYPE
		{
			DEFUALT,
			ATK0,
			ATK1,
			ATK2,
			ATK3,
			BRAVER,
		};

		void ChangeWeaponType(WEAPON_TYPE _type);
		void PlayCounterStanceEffect(bool _is_play);
		void SetCollider(bool _on);
		void SetCloud(class PlayerDataScript* _cloud) { cloud = _cloud; }
	private:
		void CreateEffect();
		void SpawnSlashHitEffect(vec3 _pos);

		class PlayerDataScript* cloud;

		vector<GameObject*> fire_effect;
		GameObject*			flash_effect;
		GameObject*			light;

		vector<Collider3D*> colliders;

		WEAPON_TYPE			type;

		vec3				prev_pos;

		float				time;

		bool				on_collider;
		bool				change;
	};
}
