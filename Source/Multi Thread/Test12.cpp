#include <iostream>
#include <thread>
#include <atomic>

#include "Utilities/Source/TimeMeasurement.h"
#include "Utilities/Source/Synchronized.h"

using namespace std::chrono_literals;

////////////////
//-- Traits --//
////////////////

template<class _C>
struct Traits;

//////////////////
//-- LoopBase --//
//////////////////

template<class _Derived>
class LoopBase
{
public:
	using ValType = typename Traits<_Derived>::ValType;

	class Iterator;

public:
	bool Run() { return this->Derived().Run(); }

	Iterator begin() { return { *this, true }; }
	Iterator end()   { return { *this, false }; }

private:
	const _Derived& Derived() const { return static_cast<const _Derived&>(*this); }
	      _Derived& Derived()       { return static_cast<      _Derived&>(*this); }
};

template<class _Derived>
class LoopBase<_Derived>::Iterator
{
public:
	using ParentType = LoopBase<_Derived>;
	using ValType    = ParentType::ValType;

	friend ParentType;

public:
	bool operator!=(const Iterator& other) const { return m_Run != other.m_Run; }

	void operator++() { m_Run = m_Parent.Run(); }

	ValType operator*() const { return {}; }

private:
	Iterator(ParentType& parent, bool run) :
		m_Parent{ parent },
		m_Run{ run }
	{}

private:
	ParentType& m_Parent;
	bool m_Run;
};

///////////////////
//-- WhileLoop --//
///////////////////

template<class _RAIIObject>
class WhileLoop : public LoopBase<WhileLoop<_RAIIObject>>
{
public:
	bool Run() const { return m_Run; }

	void Stop() { m_Run = false; }

private:
	std::atomic_bool m_Run{ true };
};

template<class _RAIIObject>
struct Traits<WhileLoop<_RAIIObject>>
{
	using ValType = _RAIIObject;
};

///////////////////
//-- Sleep for --//
///////////////////

struct SleepFor
{
	inline static thread_local std::chrono::milliseconds Intervall{ 1s };

	~SleepFor()
	{
		std::this_thread::sleep_for(Intervall);
	}
};

/////////////////////
//-- Sleep until --//
/////////////////////

struct SleepUntil
{

};

//////////////
//-- Main --//
//////////////

int main()
{
	struct LoopStruct
	{
		Timer timer;
		SleepFor sleepFor;
	};

	WhileLoop<LoopStruct> whileLoop;

	Synchronized<std::ostream&> scout{ std::cout };

	auto lambda = [&](std::chrono::milliseconds intervall)
	{
		SleepFor::Intervall = intervall;

		for (auto _ : whileLoop)
			*scout.Lock() << std::this_thread::get_id() << " - Hello world !\n";
	};

	std::thread threads[] = {
		std::thread{ lambda, 1s },
		std::thread{ lambda, 2s },
		std::thread{ lambda, 500ms }
	};

	std::cin.get();

	whileLoop.Stop();

	for (auto& t : threads)
		t.join();

	std::cout << "End\n";

	std::cin.get();
}