#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;

class Test
{
public:
	~Test()
	{
		m_Alive = false;
	}

	void Print() const
	{
		if (m_Alive)
			std::cout << "I am alive !!!" << std::endl;
		else
			std::cout << "Ded !!!" << std::endl;
	}

private:
	bool m_Alive = true;
};

int main()
{
	std::thread t1;

	{
		std::shared_ptr<Test> ptr = std::make_shared<Test>();

		std::cout << ptr.use_count() << std::endl;

		t1 = std::thread(
			[](std::shared_ptr<Test> test)
			{
				std::this_thread::sleep_for(1s);

				std::cout << test.use_count() << std::endl;

				test->Print();
			},
			ptr);

		std::cout << ptr.use_count() << std::endl;

		ptr->Print();
	}

	if (t1.joinable())
		t1.join();

	std::cin.get();
}