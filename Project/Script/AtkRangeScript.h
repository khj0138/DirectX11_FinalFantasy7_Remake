#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class AtkRangeScript : public Script
	{
	public:
		AtkRangeScript();
		~AtkRangeScript();

		CLONE(AtkRangeScript);

		virtual void Awake();
		virtual void Tick();

		virtual void BeginOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void OnOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;
		virtual void EndOverlap(Collider3D* _other, const vector<vec3>& _contact_pos) override;

		void SetPlayer(GameObject* _player) { player = _player; }

	private:
		GameObject* player;

		vector<Collider3D*> collider;
		int overlap_cnt;
	};
}
