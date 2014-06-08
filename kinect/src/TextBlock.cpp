#include "TextBlock.h"
#include "config.h"
#include "ofMain.h"
#include <random>

void TextBlock::Init(char input)
{
	m_PosY = 0.0f;
	m_Text = input;

	m_Text = char( (rand() % 26) + 'a' );
	m_SpeedConstant = 0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.0f)));
}

void TextBlock::Update(float dt)
{
	m_PosY += (TEXT_BLOCK_SPEED * m_SpeedConstant * dt);
	m_PosY = m_PosY > WINDOW_HEIGHT ? 0.0f : m_PosY;
}