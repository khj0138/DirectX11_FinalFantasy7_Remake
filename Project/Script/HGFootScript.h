#pragma once

#include "CharacterDataScript.h"

namespace ff7r
{ 
	class HGFootScript : public CharacterDataScript
	{
	public:
		HGFootScript();
		~HGFootScript();

		CLONE(HGFootScript);

		virtual void Awake() override;
		virtual void Tick() override;
		virtual void Start() override;

		virtual void Hit(ATK_TYPE _type, int _damage, vec3 _contact_pos = vec3(0.0f, 0.0f, 0.0f)) override;

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;

	private:
		GameObject* break_object;
	};
}

