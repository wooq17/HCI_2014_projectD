#pragma once

#include "ofMain.h"

class Photo
{
public:
	Photo(void);
	~Photo(void);
	
	void SetImage(const std::string& filePath);

	void SetPosition(float x, float y) { m_Position.x = x; m_Position.y = y; }
	void SetSize(float width, float height) { m_Width = width; m_Height = height; }
	void SetScale(float scale) { m_Scale = scale; }
	void Display();

	bool Touch(float x, float y);
	bool Touch(ofPoint point) { return Touch(point.x, point.y); }

	void SetPreviewSize();

private:
	ofImage m_PhotoImage;

	ofPoint				m_Position;
	float				m_Width;
	float				m_Height;
	float				m_Scale;
};

