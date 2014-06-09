#include "TextBlock.h"
#include "config.h"
#include "ofMain.h"
#include <random>

void TextBlock::Init(char input)
{
	m_PosY = TEXT_BLOCK_SIZE;
	m_Text = input;
	m_Color = ofColor(0);

	m_Text = char( (rand() % 26) + 'a' );
	m_SpeedConstant = 0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.0f)));
}

void TextBlock::Update(float dt)
{
	m_PosY += (TEXT_BLOCK_SPEED * m_SpeedConstant * dt);
	m_PosY = m_PosY > WINDOW_HEIGHT + TEXT_BLOCK_SIZE ? TEXT_BLOCK_SIZE : m_PosY;
}

void TextBlock::BeTouched(bool flag)
{
	if (flag)
	{
		m_Color = ofColor(204, 0, 0);
	}
	else
	{
		m_Color = ofColor(0);
	}
}