#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std::chrono_literals;

template<typename _DurationType>
class ScopedSleepFor
{
public:
	template<typename _DurationArgType>
	ScopedSleepFor(_DurationArgType arg) :
		m_Interval{ std::chrono::duration_cast<_DurationType>(arg) }
	{}

	~ScopedSleepFor()
	{
		std::this_thread::sleep_for(m_Interval);
	}

private:
	_DurationType m_Interval;
};

using ScopedSleepForMillis = ScopedSleepFor<std::chrono::milliseconds>;

class ForEver
{
	using _SleeperType  = ScopedSleepForMillis;
	using _DurationType = std::chrono::milliseconds;

public:
	template<typename _DurationArgType>
	ForEver(_DurationArgType arg) :
		m_Interval{ std::chrono::duration_cast<_DurationType>(arg) }
	{}

	class ForEverIterator
	{
	public:
		_SleeperType operator*() const { return { m_Loop.m_Interval }; }

		bool operator!=(const ForEverIterator& other) const { return this->m_Run != other.m_Run; }

		void operator++() { m_Run = m_Loop.s_Run; }

	private:
		ForEverIterator(const ForEver& loop, bool run = true) :
			m_Loop{ loop },
			m_Run{ run }
		{}

	private:
		const ForEver& m_Loop;
		bool m_Run = true;

		friend ForEver;
	};

	ForEverIterator begin() const { return { *this }; }

	ForEverIterator end() const { return { *this, false }; }

	void Stop() { s_Run = false; }

private:
	std::atomic_bool s_Run = true;

	_DurationType m_Interval;
};

int main()
{
	ForEver loop{ 1s };

	std::thread thread{ [&]()
	{
		for (auto _ : loop)
			std::cout << "Hello world !" << std::endl;

		std::cout << "Loop end !" << std::endl;
	} };

	std::cin.get();

	loop.Stop();

	thread.join();

	std::cin.get();
}