#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int ressource = 0;
std::shared_mutex mutex;

std::atomic_bool run = true;

void Read()
{
	std::cout << "Start reading\n";

	std::shared_lock lock(mutex);

	std::cout << ressource << std::endl;
}

void Write(int i)
{
	std::unique_lock lock(mutex);

	std::cout << "Start writing...\n";

	std::this_thread::sleep_for(2s);

	ressource = i;
}

int main()
{
	auto readLoop = []()
	{
		while (run)
			Read();
	};

	auto writeLoop = []()
	{
		int a = 0;

		while (run)
			Write(++a);
	};

	std::thread r1(readLoop);
	std::thread r2(readLoop);
	std::thread w1(writeLoop);

	std::cin.get();

	run = false;

	r1.join();
	r2.join();
	w1.join();
}