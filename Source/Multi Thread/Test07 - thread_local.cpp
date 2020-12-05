#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std::literals::chrono_literals;

#include <Utilities/Source/InstanceCounter.h>

class ThreadLocalSingleton : public Counter<ThreadLocalSingleton>
{
public:
	using AddressThreadPair = std::pair<void*, std::thread::id>;

	static ThreadLocalSingleton& Get()
	{
		thread_local ThreadLocalSingleton instance;
		return instance;
	}

	static const std::vector<AddressThreadPair>& Instances() { return m_Instances; }

private:
	ThreadLocalSingleton()
	{
		m_Instances.emplace_back((void*)(this), std::this_thread::get_id());
	}

	ThreadLocalSingleton(const ThreadLocalSingleton&) = delete;

	~ThreadLocalSingleton() = default;

	ThreadLocalSingleton& operator=(const ThreadLocalSingleton&) = delete;

	static std::vector<AddressThreadPair> m_Instances;
};

std::vector<ThreadLocalSingleton::AddressThreadPair> ThreadLocalSingleton::m_Instances;

void Thread()
{
	auto& singleton = ThreadLocalSingleton::Get();

	std::cout << ThreadLocalSingleton::CreatedObjects() << std::endl;

	std::this_thread::sleep_for(1s);
}

int main()
{
	std::thread t1(Thread);
	std::thread t2(Thread);
	std::thread t3(Thread);

	t1.join();
	t2.join();
	t3.join();

	ThreadLocalSingleton::Get();

	std::cout << "CreatedObjects : " << ThreadLocalSingleton::CreatedObjects() << std::endl;
	std::cout << "LiveObjects    : " << ThreadLocalSingleton::LiveObjects() << std::endl;

	ThreadLocalSingleton::Get();

	std::cout << "CreatedObjects : " << ThreadLocalSingleton::CreatedObjects() << std::endl;
	std::cout << "LiveObjects    : " << ThreadLocalSingleton::LiveObjects() << std::endl;

	for (auto& i : ThreadLocalSingleton::Instances())
		std::cout << i.first << ", " << i.second << std::endl;

	std::cin.get();
}