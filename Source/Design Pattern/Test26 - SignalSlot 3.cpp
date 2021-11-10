#include <iostream>

#pragma region Type indexing

template<typename Type>
class TypeIndexer;

class TypeAutoInc
{
private:
	static size_t Next()
	{
		static size_t i = 0;
		return i++;
	}

	template<typename Type>
	friend class TypeIndexer;
};

template<typename Type>
class TypeIndexer
{
public:
	static size_t Index()
	{
		static size_t idx = TypeAutoInc::Next();
		return idx;
	}
};

#pragma endregion

class SignalSlotSystem
{
public:
	static SignalSlotSystem& Get() noexcept
	{
		static SignalSlotSystem instance;
		return instance;
	}

private:
	SignalSlotSystem()  = default;
	~SignalSlotSystem() = default;

	SignalSlotSystem(const SignalSlotSystem&) = delete;
	SignalSlotSystem& operator=(const SignalSlotSystem&) = delete;
};

int main()
{
	SignalSlotSystem::Get();

	std::cout << TypeIndexer<int>::Index() << std::endl;
	std::cout << TypeIndexer<char>::Index() << std::endl;
	std::cout << TypeIndexer<float>::Index() << std::endl;
	std::cout << TypeIndexer<void()>::Index() << std::endl;
	std::cout << TypeIndexer<int(int)>::Index() << std::endl;

	std::cin.get();
}