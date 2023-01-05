#include "main.h"
#include "input.h"

#define KEY_NONE NULL

static const POINT SCREEN_CENTER = { (int)SCREEN_WIDTH_HALF,(int)SCREEN_HEIGHT_HALF };

KEY_CONFIG_INFO keyConfigList[] =
{
	{KEY_CONFIG::MOVE_UP   ,		KEY_TYPE::OR,	'W',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOVE_DOWN ,		KEY_TYPE::OR,	'S',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOVE_LEFT ,		KEY_TYPE::OR,	'A',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOVE_RIGHT,		KEY_TYPE::OR,	'D',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::JUMP      ,		KEY_TYPE::OR,	VK_SPACE,	KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::DETERMINATION,		KEY_TYPE::OR,	'E',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOUSE_L   ,		KEY_TYPE::OR,	VK_LBUTTON,	KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOUSE_R   ,		KEY_TYPE::OR,	VK_RBUTTON,	KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOUSE_OPERATION,	KEY_TYPE::OR,	VK_LCONTROL,KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::OPEN_UI   ,		KEY_TYPE::OR,	VK_TAB,		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::OBJECT_ROTATE_L,	KEY_TYPE::OR,	'Q',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::OBJECT_ROTATE_R,	KEY_TYPE::OR,	'R',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::RETURN	   ,		KEY_TYPE::OR,	VK_RETURN,	KEY_NONE,KEY_NONE,KEY_NONE},
};

void Input::Init()
{
	for (int i = 0; i < KEY_NUM; i++)
	{
		m_oldKeyState[i] = KEY_STATE::NONE;
		m_keyState[i] = KEY_STATE::NONE;
	}

	ShowCursor(false);
}

void Input::Uninit()
{
}

void Input::Update()
{
	BYTE buffer[KEY_NUM];
	//前回のキー入力保存
	for (int i = 0; i < KEY_NUM; i++)
	{
		m_oldKeyState[i] = m_keyState[i];
	}
	//全キー入力取得
	(void)GetKeyboardState(buffer);

	for (int i = 0; i < KEY_NUM; i++)
	{
		//押されているか
		if (buffer[i] & 0x80)
		{
			//前回入力されていなかったらTRIGGERに
			if (m_oldKeyState[i] == KEY_STATE::NONE)
				m_keyState[i] = KEY_STATE::TRIGGER;
			else
				m_keyState[i] = KEY_STATE::PRESS;
		}
		else
		{
			//前回押されていたら
			if (m_oldKeyState[i] == KEY_STATE::PRESS || m_oldKeyState[i] == KEY_STATE::TRIGGER)
				m_keyState[i] = KEY_STATE::RELEASE;
			else
				m_keyState[i] = KEY_STATE::NONE;
		}
	}

	//マウス用
	if (GetKeyTrigger(KEY_CONFIG::MOUSE_OPERATION))
		ShowCursor(true);
	else if (GetKeyRelease(KEY_CONFIG::MOUSE_OPERATION))
	{
		ShowCursor(false);
		SetCursorPosToClient(SCREEN_CENTER);
	}
	else if (GetKeyPress(KEY_CONFIG::MOUSE_OPERATION))
	{
		m_mouseAcceleration = Vector2(0.0f, 0.0f);
	}
	else
	{
		m_preMousePos = m_mousePos;
		auto pos = MouseAcquisition();
		m_mousePos = Vector2(pos.x, pos.y);
		m_mouseAcceleration = Vector2(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF) - m_mousePos;
		SetCursorPosToClient(SCREEN_CENTER);
	}
}

bool Input::GetKeyPress(KEY_CONFIG key)
{
	return CheckKey(key, KEY_STATE::PRESS);
}

bool Input::GetKeyTrigger(KEY_CONFIG key)
{
	return CheckKey(key, KEY_STATE::TRIGGER);
}

bool Input::GetKeyRelease(KEY_CONFIG key)
{
	return CheckKey(key, KEY_STATE::RELEASE);
}

bool Input::CheckKey(KEY_CONFIG keyConfig, KEY_STATE keyState)
{
	for (int i = 0; i < KEY_CONFIG_ASSIGN_NUM; i++)
	{
		auto config = keyConfigList[static_cast<int>(keyConfig)];

		if (config.keyCode[i] == KEY_NONE)
		{
			return false;
		}

		if (config.type == KEY_TYPE::OR)
		{
			if (m_keyState[config.keyCode[i]] == keyState)
			{
				return true;
			}
		}
		else
		{
			if (m_keyState[config.keyCode[i]] != keyState)
			{
				return false;
			}
			return true;
		}
	}

	return false;
}