#include <iostream>
#include <functional>

#define NOMINMAX
#include <Windows.h>

#define PRINTVAL(X) std::cout << #X << " = " << X << std::endl

int main()
{
	HANDLE       hStdin = GetStdHandle(STD_INPUT_HANDLE);
	HWND         hWnd = GetConsoleWindow();
	DWORD        cNumRead;
	INPUT_RECORD irInBuf[128];

	SetConsoleMode(hStdin, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

	bool run = true;

	struct KeyState
	{
		bool KeyPressed  : 1;
		bool KeyReleased : 1;
		bool KeyHeld     : 1;
		bool _NA         : 3;
		bool KeyOldState : 1;
		bool KeyNewState : 1;
	} keys[256];

	std::memset(keys, 0, 256 * sizeof(KeyState));

	using KeyBoardAction = std::function<void(const KeyState&)>;

	KeyBoardAction keyactions[256];

	keyactions[VK_ESCAPE] = [&run](const KeyState&) { run = false; };
	keyactions[VK_SPACE]  = [](const KeyState& key)
	{
		if(key.KeyPressed)
			std::cout << "Jump !" << std::endl;
	};

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
				key.KeyHeld     = key.KeyNewState;
				key.KeyPressed  = !key.KeyOldState && key.KeyNewState;
				key.KeyReleased = key.KeyOldState && !key.KeyNewState;

				if(keyaction)
					keyaction(key);

				key.KeyOldState = key.KeyNewState;

				break;
			}
			default:
				break;
			}
		}
	}
}