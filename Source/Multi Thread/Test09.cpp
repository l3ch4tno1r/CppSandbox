#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

#include "Utilities/Source/Instrumentor.h"

using namespace std::chrono_literals;

#define SEPARATOR(X) std::cout << "--------- " << X << " ---------" << std::endl;

int main()
{
	timeBeginPeriod(1);

	Instrumentor::Get().BeginSession("Test");

	SEPARATOR("Test 01");
	{
		PROFILE_SCOPE("Test 01");

		std::chrono::system_clock::time_point now, next, last;
		last = std::chrono::system_clock::now();

		for (int i = 0; i < 20; i++)
		{
			PROFILE_SCOPE("For loop");

			{
				PROFILE_SCOPE("Block 1");

				now = std::chrono::system_clock::now();
				float dt = (float)std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
				next = now + 8ms;
				float dtTh = (float)std::chrono::duration_cast<std::chrono::milliseconds>(next - now).count();

				{
					PROFILE_SCOPE("Block 2");

					std::cout << dt << ", " << dtTh << ", delta = " << dt - dtTh << std::endl;
					//std::cout << now.time_since_epoch().count() << ", " << next.time_since_epoch().count() << std::endl;
				}
			}

			{
				PROFILE_SCOPE("Block 3");

				std::this_thread::sleep_until(next);
				//std::this_thread::sleep_for(128ms);
			}

			last = now;
		}
	}

	/*
	SEPARATOR("Test 02");
	{
		PROFILE_SCOPE("Test 02");

		for (int i = 0; i < 20; i++)
		{
			PROFILE_SCOPE("For loop");

			std::this_thread::sleep_for(16ms);
		}
	}
	*/

	/*
	SEPARATOR("Test 03");
	{
		std::cout << "Hello waiter\n" << std::flush;

		auto start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(16ms);
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double, std::milli> elapsed = end - start;
		std::cout << "Waited " << elapsed.count() << " ms\n";
	}
	*/

	Instrumentor::Get().EndSession();

	timeEndPeriod(1);

	std::cin.get();
}