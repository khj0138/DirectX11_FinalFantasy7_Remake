#pragma once

#include "Component.h"

namespace ff7r
{
	class Socket : public Component
	{
	public:
		Socket();
		Socket(const Socket& _Other);
		~Socket();

		CLONE(Socket);

		virtual void Begin() override;
		virtual void FinalTick() override;

		void SetSocket(UINT _idx) { socket_idx = _idx; }
		void SetSocket(wstring _name);

	private:
		virtual void SaveToLevelFile(FILE* _file) override;
		virtual void LoadFromLevelFile(FILE* _file) override;

		UINT socket_idx;
	};
}

