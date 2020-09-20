#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std::literals::chrono_literals;

#define NOMINMAX
#include <Windows.h>

#define PRINTVAL(X) std::cout << #X << " = " << X << std::endl

std::atomic<bool> run = true;

void Thread()
{
	while (run)
	{
		auto start = std::chrono::high_resolution_clock::now();

		std::cout << "Working" << std::endl;

		std::this_thread::sleep_until(start + 1s);
	}
}

int main()
{
	HANDLE       hStdin = GetStdHandle(STD_INPUT_HANDLE);
	HWND         hWnd = GetConsoleWindow();
	DWORD        cNumRead;
	INPUT_RECORD irInBuf[128];

	//std::thread thread(Thread);

	SetConsoleMode(hStdin, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

	struct KeyState
	{
		bool KeyPressed  : 1;
		bool KeyReleased : 1;
		bool KeyHeld     : 1;
		bool _NA         : 3;
		bool KeyOldState : 1;
		bool KeyNewState : 1;
	} keys[256], mouse[5];

	std::memset(keys, 0, 256 * sizeof(KeyState));
	std::memset(mouse, 0, 5 * sizeof(KeyState));

	using KeyBoardAction = std::function<void(const KeyState&)>;
	using MouseAction    = std::function<void(const KeyState&, int, int)>;

	KeyBoardAction keyactions[256];

	keyactions[VK_ESCAPE] = [](const KeyState&) { run = false; };
	keyactions[VK_SPACE]  = [](const KeyState& key)
	{
		if(key.KeyPressed)
			std::cout << "Jump !" << std::endl;
	};

	MouseAction mouseactions[5];

	for (int i = 0; i < 5; ++i)
	{
		mouseactions[i] = [i](const KeyState& mstate, int x, int y)
		{
			if(mstate.KeyPressed)
				std::cout << i << " pressed : (" << x << ", " << y << ')' << std::endl;

			if (mstate.KeyReleased)
				std::cout << i << " released : (" << x << ", " << y << ')' << std::endl;
		};
	}

	while (run)
	{
		cNumRead = 0;
		ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead);

		for (DWORD i = 0; i < cNumRead; i++)
		{
			const INPUT_RECORD& record = irInBuf[i];

			switch (record.EventType)
			{
			case KEY_EVENT:
			{
				KeyState&       key = keys[record.Event.KeyEvent.wVirtualKeyCode];
				KeyBoardAction& keyaction = keyactions[record.Event.KeyEvent.wVirtualKeyCode];

				key.KeyNewState = record.Event.KeyEvent.bKeyDown;
				key.KeyHeld     =  key.KeyNewState;
				key.KeyPressed  = !key.KeyOldState &&  key.KeyNewState;
				key.KeyReleased =  key.KeyOldState && !key.KeyNewState;

				if(keyaction)
					keyaction(key);

				key.KeyOldState = key.KeyNewState;

				break;
			}
			case MOUSE_EVENT:
			{
				switch (record.Event.MouseEvent.dwEventFlags)
				{
				case MOUSE_MOVED:
				{
					break;
				}
				case 0:
				{
					for (int i = 0; i < 5; ++i)
					{
						mouse[i].KeyNewState = (record.Event.MouseEvent.dwButtonState & (1 << i)) > 0;
						mouse[i].KeyHeld     =  mouse[i].KeyNewState;
						mouse[i].KeyPressed  = !mouse[i].KeyOldState &&  mouse[i].KeyNewState;
						mouse[i].KeyReleased =  mouse[i].KeyOldState && !mouse[i].KeyNewState;

						int x = record.Event.MouseEvent.dwMousePosition.X;
						int y = record.Event.MouseEvent.dwMousePosition.Y;

						if (mouseactions[i])
							mouseactions[i](mouse[i], x, y);

						mouse[i].KeyOldState = mouse[i].KeyNewState;
					}

					break;
				}
				default:
					break;
				}

				break;
			}
			default:
				break;
			}
		}
	}

	//thread.join();
}