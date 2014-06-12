#include "PhotoAlbum.h"

#include "Photo.h"
#include "config.h"
#include <math.h>

PhotoAlbum::PhotoAlbum(void)
{
	m_Position.x = WINDOW_WIDTH / 2;
	m_Position.y = WINDOW_HEIGHT / 2;

	m_Width = 0.0f;
	m_Height = 0.0f;

	m_Scale = 1.0f;
	m_Visible = true;
}


PhotoAlbum::~PhotoAlbum(void)
{
	std::for_each(m_PhotoList.begin(), m_PhotoList.end(), [](Photo* each)
	{
		delete each;
	}
	);
}

void PhotoAlbum::Reset()
{
	std::for_each(m_PhotoList.begin(), m_PhotoList.end(), [&](Photo* each)
	{
		each->SetPreviewSize();
		each->SetPosition(m_Position.x, m_Position.y);
	}
	);
}

void PhotoAlbum::SetAlbumImage(const std::string& filePath)
{
	if ( !m_AlbumImage.loadImage(filePath))
	{
		ofLog(OF_LOG_ERROR, "cannot open the image");
	}

	m_Width = m_AlbumImage.getWidth();
	m_Height = m_AlbumImage.getHeight();
}

void PhotoAlbum::AddPhoto(Photo* newPhoto)
{
	m_PhotoList.push_back(newPhoto);
}

void PhotoAlbum::DeletePhoto(Photo* targetPhoto)
{
	vector<Photo*>::const_iterator location = std::find(m_PhotoList.begin(), m_PhotoList.end(), targetPhoto);

	if (location != m_PhotoList.end())
	{
		m_PhotoList.erase(location);
	}
}

bool PhotoAlbum::Touch(float x, float y)
{
	if (x > m_Position.x - (m_Width * m_Scale)/2 &&
		x < m_Position.x + (m_Width * m_Scale)/2 &&
		y > m_Position.y - (m_Height * m_Scale)/2 &&
		y < m_Position.y + (m_Height * m_Scale)/2)
	{
		// 앨범 안 이미지들의 위치를 바꾼다.
		int imageCount = m_PhotoList.size();
		double angle = 3.1415926 * 2 / imageCount;

		int i = 0;
		std::for_each(m_PhotoList.begin(), m_PhotoList.end(), [&](Photo* each)
		{
			each->SetPosition(m_Position.x + cos(angle * i) * PREVIEW_DISTANCE, m_Position.y + sin(angle * i) * PREVIEW_DISTANCE );
			++i;
		}
		);

		return true;
	}

	return false;
}


void PhotoAlbum::Display(bool isOpen)
{
	if (!m_Visible)
		return;

	m_AlbumImage.draw(
		m_Position.x - (m_Width * m_Scale)/2, 
		m_Position.y - (m_Height * m_Scale)/2, 
		m_Width * m_Scale, 
		m_Height * m_Scale);

	if (isOpen)
	{
		std::for_each(m_PhotoList.begin(), m_PhotoList.end(), [&](Photo* each)
		{
			each->Display();
		}
		);
	}
}
