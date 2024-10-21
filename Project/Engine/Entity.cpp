#include "pch.h"
#include "Entity.h"

namespace ff7r
{
	UINT Entity::g_next_id = 0;

	Entity::Entity()
		: id(g_next_id++)
	{
	}

	Entity::Entity(const Entity& _other)
		: name(_other.name)
		, id(g_next_id++)
	{
	}

	Entity::~Entity()
	{
	}
}