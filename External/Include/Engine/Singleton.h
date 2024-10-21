#pragma once
namespace ff7r
{
	typedef void (*EXIT)(void);

	template<typename T>
	class Singleton
	{
	private:
		static T* g_inst;

	public:
		static T* GetInst();
		static void Destroy();

	protected:
		Singleton() { }
		virtual ~Singleton() {}
	};

	template<typename T>
	T* Singleton<T>::g_inst = nullptr;

	template<typename T>
	inline T* Singleton<T>::GetInst()
	{
		if (nullptr == g_inst)
		{
			g_inst = new T;
			atexit((EXIT)&Singleton<T>::Destroy);
		}

		return g_inst;
	}

	template<typename T>
	inline void Singleton<T>::Destroy()
	{
		if (nullptr != g_inst)
		{
			delete g_inst;
			g_inst = nullptr;
		}
	}
}