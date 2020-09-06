#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "Source/InstanceCounter.h"
#include "Source/Design Patterns/Observer.h"
#include "Source/TraceLog.h"

using namespace std::literals::chrono_literals;
using namespace LCNUtilities;

#define SEPARATOR(X) Log() << "------------- " << X << " -------------"
#define SIZEOF(X) Log() << "Size of " << #X << " : " << sizeof(X)

class Device : public Observer<Device>
{
private:
	std::thread m_MainThread;

	struct
	{
		bool Run      : 1;
		bool Notified : 1;
	} m_State;

	std::mutex m_NotifMut;
	std::condition_variable m_NotifCond;

	void MainThread()
	{
		while (Continue())
			Log() << std::this_thread::get_id() << " working...";

		Log() << std::this_thread::get_id() << " Done !";
	}

	bool Continue()
	{
		std::unique_lock<std::mutex> lock(m_NotifMut);

		while (!m_State.Notified)
			m_NotifCond.wait(lock);

		m_State.Notified = false;

		return m_State.Run;
	}

public:
	~Device()
	{
		Update(false);

		if (m_MainThread.joinable())
			m_MainThread.join();
	}

	void Start()
	{
		if (m_State.Run)
			return;

		m_State.Run = true;

		m_MainThread = std::thread(&Device::MainThread, this);
	}

	void Update(bool run)
	{
		std::lock_guard<std::mutex> lock(m_NotifMut);

		m_State.Run     = run;
		m_State.Notified = true;

		m_NotifCond.notify_one();
	}
};

class PaceMaker : public Subject<Device>
{
private:
	bool m_Run = false;
	std::thread m_MainThread;

	void MainThread()
	{
		while (m_Run)
		{
			Log() << "Heartbeat !";

			this->Notify(true);

			std::this_thread::sleep_for(1s);
		}
	}

public:
	~PaceMaker()
	{
		m_Run = false;

		if (m_MainThread.joinable())
			m_MainThread.join();
	}

	void Start()
	{
		m_Run = true;

		m_MainThread = std::thread(&PaceMaker::MainThread, this);
	}
};

int main()
{
	/*
	{
		SEPARATOR(1);

		PaceMaker subject;

		Device obs1;
		Device obs2;
		Device obs3;

		obs1.Start();
		obs2.Start();
		obs3.Start();

		subject.AddObserver(obs1);
		subject.AddObserver(obs2);
		subject.AddObserver(obs3);

		subject.Start();

		std::cin.get();
	}
	*/

	{
		SEPARATOR(2);

		SIZEOF(Device);
		SIZEOF(Observer<Device>);
		SIZEOF(std::thread);
		SIZEOF(std::mutex);
		SIZEOF(std::condition_variable);
	}

	SEPARATOR("Done");

	std::cin.get();
}