#pragma once

namespace ff7r
{
	class GameObject;

	class BaseState
	{
	public:
		virtual void Enter() = 0;
		virtual void Excute() = 0;
		virtual void Exit() = 0;
	};
}

