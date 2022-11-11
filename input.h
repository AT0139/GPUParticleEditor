#pragma once


class Input
{
private:
	static BYTE m_oldKeyState[256];
	static BYTE m_keyState[256];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

};
