#pragma once

#include "Singleton.h"

#define KEY_NUM 256
#define KEY_CONFIG_ASSIGN_NUM 4


enum class KEY_CONFIG
{
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,

	JUMP,

	DETERMINATION,

	MOUSE_L,
	MOUSE_R,
	MOUSE_OPERATION,

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

class Input : public Singleton<Input>
{
public:
	friend class Singleton<Input>;

	Input();
	~Input() {}

	void Init();
	void Uninit();
	void Update();

	bool GetKeyPress(KEY_CONFIG key);
	bool GetKeyTrigger(KEY_CONFIG key);
	bool GetKeyRelease(KEY_CONFIG key);

	Vector2 GetMousePoint() { return m_mousePos; }
	Vector2 GetMouseAcceleration() { return m_mouseAcceleration; }

	void ToggleCursor(bool show);
private:
	bool CheckKey(KEY_CONFIG keyConfigList, KEY_STATE keyState);

	KEY_STATE m_oldKeyState[KEY_NUM];
	KEY_STATE m_keyState[KEY_NUM];

	Vector2 m_mousePos;
	Vector2 m_preMousePos;
	Vector2 m_mouseAcceleration;

	bool m_enebleCursor;
};
