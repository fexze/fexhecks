#pragma once

#include "pIncludes.hpp"
#include "Singleton.hpp"

using namespace std;

enum class KeyState
{
	None = 1,
	Down,
	Up,
	Pressed
};

DEFINE_ENUM_FLAG_OPERATORS(KeyState);

class InputSys : public Singleton<InputSys>
{
	friend class Singleton<InputSys>;
	InputSys();
	~InputSys();

public:
	void Initialize();

	HWND GetMainWindow() const
	{
		return m_hTargetWindow;
	}

	bool        IsKeyDown(uint32_t vk);
	KeyState    GetKeyState(uint32_t vk);
	bool        WasKeyPressed(uint32_t vk);
	void        RemoveHotkey(uint32_t vk);
	void        RegisterHotkey(uint32_t vk, std::function<void(void)> f);

private:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	KeyState        m_iKeyMap[256];
	LONG_PTR        m_ulOldWndProc;
	HWND            m_hTargetWindow;

	std::function<void(void)> m_Hotkeys[256];
};