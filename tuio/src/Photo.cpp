#include "Photo.h"
#include "config.h"

Photo::Photo(void)
{
	m_Position.x = WINDOW_WIDTH / 2;
	m_Position.y = WINDOW_HEIGHT / 2;

	m_Width = 0.0f;
	m_Height = 0.0f;

	m_Scale = 1.0f;
	m_RotateAngle = 0.0f;
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

void Photo::Reset()
{
	m_Position.x = WINDOW_WIDTH / 2;
	m_Position.y = WINDOW_HEIGHT / 2;

	float tempScaleX = WINDOW_WIDTH / m_Width;
	float tempScaleY = WINDOW_HEIGHT / m_Height;

	m_Scale = (tempScaleX < tempScaleY) ? tempScaleX : tempScaleY;
	m_Scale = (m_Scale > 1.0f) ? 1.0f : m_Scale;

	m_RotateAngle = 0.0f;
}

void Photo::SetPreviewSize()
{
	float tempScaleX = PREVIEW_IMG_SIZE / m_Width;
	float tempScaleY = PREVIEW_IMG_SIZE / m_Height;

	m_Scale = (tempScaleX < tempScaleY) ? tempScaleX : tempScaleY;

	m_RotateAngle = 0.0f;
}

bool Photo::Touch(float x, float y)
{
	/*
	if (x > m_Position.x - (m_Width * m_Scale)/2 &&
		x < m_Position.x + (m_Width * m_Scale)/2 &&
		y > m_Position.y - (m_Height * m_Scale)/2 &&
		y < m_Position.y + (m_Height * m_Scale)/2)
	{
		return true;
	}

	return false;
	*/
	if (x < m_Position.x - (m_Width * m_Scale)/2)
		return false;

	if (x > m_Position.x + (m_Width * m_Scale)/2)
		return false;
		
	if (y < m_Position.y - (m_Height * m_Scale)/2)
		return false;

	if (y > m_Position.y + (m_Height * m_Scale)/2)
		return false;

	return true;
}

void Photo::SetAngle(float angle) 
{ 
	if (angle > -5 && angle < 5)
		m_RotateAngle = 0.0f;
	else
		m_RotateAngle = angle; 
}

void Photo::Display()
{
	ofPushMatrix();
		ofTranslate(m_Position.x, m_Position.y, 0);
		ofRotate(m_RotateAngle, 0, 0, 1);
		ofPushMatrix();
			m_PhotoImage.draw(
				- (m_Width * m_Scale)/2, 
				- (m_Height * m_Scale)/2, 
				m_Width * m_Scale, 
				m_Height * m_Scale);
		ofPopMatrix();
	ofPopMatrix();
}
