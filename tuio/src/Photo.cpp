#include "Photo.h"
#include "config.h"

Photo::Photo(void)
{
	m_Position.x = WINDOW_WIDTH / 2;
	m_Position.y = WINDOW_HEIGHT / 2;

	m_Width = 0.0f;
	m_Height = 0.0f;

	m_Scale = 1.0f;
}


Photo::~Photo(void)
{
}

void Photo::SetImage(const std::string& filePath)
{
	if ( !m_PhotoImage.loadImage(filePath))
	{
		ofLog(OF_LOG_ERROR, "cannot open the image");
	}

	m_Width = m_PhotoImage.getWidth();
	m_Height = m_PhotoImage.getHeight();

	SetPreviewSize();
}

void Photo::SetPreviewSize()
{
	float tempScaleX = PREVIEW_IMG_SIZE / m_Width;
	float tempScaleY = PREVIEW_IMG_SIZE / m_Height;

	m_Scale = (tempScaleX < tempScaleY) ? tempScaleX : tempScaleY;
}

bool Photo::Touch(float x, float y)
{
	if (x > m_Position.x - (m_Width * m_Scale)/2 &&
		x < m_Position.x + (m_Width * m_Scale)/2 &&
		y > m_Position.y - (m_Height * m_Scale)/2 &&
		y < m_Position.y + (m_Height * m_Scale)/2)
	{
		return true;
	}

	return false;
}


void Photo::Display()
{
	m_PhotoImage.draw(
		m_Position.x - (m_Width * m_Scale)/2, 
		m_Position.y - (m_Height * m_Scale)/2, 
		m_Width * m_Scale, 
		m_Height * m_Scale);
}
