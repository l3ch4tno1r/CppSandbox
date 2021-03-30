#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

class Test
{
public:
	~Test()
	{
		if (m_Thread.joinable())
			m_Thread.join();

		std::cout << "Joined" << std::endl;
		std::cin.get();
	}

	static Test& Get()
	{
		static Test instance;
		return instance;
	}

	void Start()
	{
		m_Thread = std::thread([]()
			{
				std::this_thread::sleep_for(5s);

				std::cout << "Done" << std::endl;
			});
	}

private:
	std::thread m_Thread;
};

int main()
{
	Test::Get().Start();

	std::cin.get();
}