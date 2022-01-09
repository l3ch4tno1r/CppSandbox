#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <type_traits>

template<typename _Type>
class Sync
{
public:
	using ValType = std::remove_reference<_Type>::type;

	class SyncLock : public std::lock_guard<std::mutex>
	{
	public:
		using BaseType = std::lock_guard<std::mutex>;

		ValType& operator*() { return m_Ref; }

		ValType* operator->() { return &m_Ref; }

	private:
		SyncLock(ValType& ref, std::mutex& mut) :
			m_Ref{ ref },
			BaseType{ mut }
		{}

	private:
		ValType& m_Ref;

		friend Sync;
	};

	template<typename ... Args>
	Sync(Args&& ... args) :
		m_Data(std::forward<Args>(args)...)
	{}

	SyncLock Lock() { return { m_Data, m_Mut }; }

private:
	_Type m_Data;
	std::mutex m_Mut;
};

int main()
{
	Sync<std::ofstream> sof{ "Test02.txt", std::ios::out };
	//std::ofstream os{ "Test01.txt", std::ios::out };

	if (!(*sof.Lock()))
		return -1;

	auto lambda = [&]()
	{
		for(int i = 0; i < 1000; ++i)
			*sof.Lock() << std::this_thread::get_id() << " Hello world ! #" << i << std::endl;
	};

	std::thread threads[] = {
		std::thread{ lambda },
		std::thread{ lambda },
		std::thread{ lambda },
		std::thread{ lambda }
	};

	for (auto& t : threads)
		t.join();

	std::cin.get();
}