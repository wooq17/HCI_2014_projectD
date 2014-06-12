#pragma once

#include "ofMain.h"

class Photo;

class PhotoAlbum
{
public:
	PhotoAlbum(void);
	~PhotoAlbum(void);
	
	void Close();
	void Open();

	void SetAlbumImage(const std::string& filePath);
	void AddPhoto(Photo* newPhoto);
	void DeletePhoto(Photo* targetPhoto);

	void SetPosition(float x, float y) { m_Position.x = x; m_Position.y = y; }
	void MovePosition(float x, float y) { m_Position.x += x; m_Position.y += y; }
	void SetSize(float width, float height) { m_Width = width; m_Height = height; }
	void SetScale(float scale) { m_Scale = scale; }
	void Display(bool isOpen);

	// 이미지 범위 안을 선택하면 선택
	// 이미지 범위를 벗어난 좌표면 false리턴
	bool Touch(float x, float y);
	bool Touch(ofPoint point) { return Touch(point.x, point.y); }

	Photo* GetSelectedPhoto(float x, float y);
	Photo* GetSelectedPhoto(ofPoint point) { return GetSelectedPhoto(point.x, point.y); }

private:
	ofImage				m_AlbumImage;
	std::vector<Photo*>	m_PhotoList;

	ofPoint				m_Position;
	float				m_Width;
	float				m_Height;

	float				m_Scale;

	bool				m_Visible;
};

