#include "pch.h"
#include "Component.h"

namespace ff7r
{
	Component::Component(COMPONENT_TYPE _Type)
		: owner(nullptr)
		, type(_Type)
		, enabled(true)
	{
	}

	Component::Component(const Component& _Other)
		: Entity(_Other)
		, owner(nullptr)
		, type(_Other.type)
		, enabled(true)
	{
	}

	Component::~Component()
	{
	}

	void Component::SetEnabled(bool _enable)
	{
		if (_enable != enabled)
		{
			enabled = _enable;

			if (_enable)
			{
				OnEnable();
			}
			else
			{
				OnDisable();
			}
		}
	}
}