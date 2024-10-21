#pragma once

namespace ff7r
{
	class Entity
	{
	private:
		static UINT g_next_id;

	private:
		wstring		name;
		const UINT  id;

	public:
		void SetName(const wstring& _strName) { name = _strName; }
		const wstring& GetName() { return name; }
		UINT GetID() { return id; }

	public:
		virtual Entity* Clone() = 0;

	public:
		Entity();
		Entity(const Entity& _other);
		virtual ~Entity();
	};
}

