#pragma once

#include <string>

class TextBlock
{
public:
	TextBlock() 
	{
		m_PosY = 0.0f;
		m_SpeedConstant = 1.0f;
	}
	~TextBlock() {}
	
	void Init(char input);
	void Update(float dt);

	void SetPosition(float posY) { m_PosY = posY; }
	float GetPosition() { return m_PosY; }

	std::string GetText() { return m_Text; }

private:
	std::string m_Text;
	float m_PosY;
	float m_SpeedConstant;
};