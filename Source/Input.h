#pragma once

#define KEY_NUM 256
#define KEY_CONFIG_ASSIGN_NUM 4

enum class KEY_CONFIG
{
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,

	JUMP,
	ACTION,

	MOUSE_L,
	MOUSE_R,

	OPEN_UI,
	OBJECT_ROTATE_L,
	OBJECT_ROTATE_R,

	RETURN,

	MAX
};

enum class KEY_TYPE
{
	AND,
	OR
};

enum class KEY_STATE
{
	TRIGGER,
	PRESS,
	RELEASE,
	NONE,
};

struct KEY_CONFIG_INFO
{
	KEY_CONFIG keyConfigList;
	KEY_TYPE type;
	BYTE keyCode[KEY_CONFIG_ASSIGN_NUM];
};

class Input
{
public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress(KEY_CONFIG key);
	static bool GetKeyTrigger(KEY_CONFIG key);
	static bool GetKeyRelease(KEY_CONFIG key);

private:
	static bool CheckKey(KEY_CONFIG keyConfigList, KEY_STATE keyState);

	static KEY_STATE m_oldKeyState[KEY_NUM];
	static KEY_STATE m_keyState[KEY_NUM];
};
