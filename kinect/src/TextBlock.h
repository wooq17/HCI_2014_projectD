#pragma once

#include <string>
#include "ofMain.h"

class TextBlock
{
public:
	TextBlock() 
	{
		m_PosY = 20.0f;
		m_SpeedConstant = 1.0f;
	}
	~TextBlock() {}
	
	void Init(char input);
	void Update(float dt);

	void SetPosition(float posY) { m_PosY = posY; }
	float GetPosition() { return m_PosY; }

	void BeTouched(bool flag);
	ofColor GetColor() { return m_Color; }

	std::string GetText() { return m_Text; }

private:
	std::string m_Text;
	ofColor m_Color;
	float m_PosY;
	float m_SpeedConstant;
};