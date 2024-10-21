#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class MagicFireTrailScript : public Script
	{
	public:
		MagicFireTrailScript();
		~MagicFireTrailScript();

		CLONE(MagicFireTrailScript);

		virtual void Awake() override;
		
		virtual void OnEnable() override;
	private:

	};
}
