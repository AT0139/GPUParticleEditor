#pragma once

class GameObject
{
public:
	GameObject() {}
	virtual ~GameObject() {}

	//èÉêàâºëzä÷êî
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetPosition(D3DXVECTOR3 position) { m_position = position; }
	void SetDestroy() { m_destory = true; }
	bool Destroy()
	{
		if (m_destory)
		{
			Uninit();
			delete this;
			return true;
		}
		else 
			return false;
	}
	
protected:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;


private:
	bool m_destory = false;
};