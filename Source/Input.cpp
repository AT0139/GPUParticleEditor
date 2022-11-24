#include "main.h"
#include "input.h"

#define KEY_NONE NULL

KEY_STATE Input::m_oldKeyState[256];
KEY_STATE Input::m_keyState[256];

KEY_CONFIG_INFO keyConfigList[] =
{
	{KEY_CONFIG::MOVE_UP   ,	KEY_TYPE::OR,	'W',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOVE_DOWN ,	KEY_TYPE::OR,	'S',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOVE_LEFT ,	KEY_TYPE::OR,	'A',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::MOVE_RIGHT,	KEY_TYPE::OR,	'D',		KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::JUMP      ,	KEY_TYPE::OR,	VK_SPACE,	KEY_NONE,KEY_NONE,KEY_NONE},
	{KEY_CONFIG::RETURN	   ,	KEY_TYPE::OR,	VK_RETURN,	KEY_NONE,KEY_NONE,KEY_NONE},
};

void Input::Init()
{
	memset(m_oldKeyState, 0, 256);
	memset(m_keyState, 0, 256);
}

void Input::Uninit()
{
}

void Input::Update()
{
	BYTE buffer[KEY_NUM];
	//前回のキー入力保存
	memcpy(m_oldKeyState, m_keyState, KEY_NUM);
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
}

bool Input::GetKeyPress(KEY_CONFIG key)
{
	return CheckKey(key, KEY_STATE::PRESS);
}

bool Input::GetKeyTrigger(KEY_CONFIG key)
{
	return CheckKey(key, KEY_STATE::TRIGGER);
}

bool Input::CheckKey(KEY_CONFIG keyConfig, KEY_STATE keyState)
{
	for (int i = 0; i < KEY_CODE_NUM; i++)
	{
		auto config = keyConfigList[static_cast<int>(keyConfig)];
		if (config.type == KEY_TYPE::OR)
		{
			if (m_keyState[config.keyCode[i]] == keyState)
			{
				return true;
			}
			return false;
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

	return true;
}