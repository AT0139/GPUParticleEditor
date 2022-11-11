
#include "main.h"
#include "input.h"


BYTE Input::m_oldKeyState[256];
BYTE Input::m_keyState[256];


void Input::Init()
{

	memset( m_oldKeyState, 0, 256 );
	memset( m_keyState, 0, 256 );

}

void Input::Uninit()
{


}

void Input::Update()
{

	memcpy( m_oldKeyState, m_keyState, 256 );

	(void)GetKeyboardState( m_keyState );

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_keyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_keyState[KeyCode] & 0x80) && !(m_oldKeyState[KeyCode] & 0x80));
}
