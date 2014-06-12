#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxTuioClient.h"
#include "config.h"
#include <array>
#include "PhotoAlbum.h"

class testApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofxTuioClient myTuio;
	
	void touchDown(ofTouchEventArgs & touch);
	void touchUp(ofTouchEventArgs & touch);
	void touchMoved(ofTouchEventArgs & touch);

private:
	void SingleTap();
	void DoubleTap();
	void Drag();
	void Pinch();
	void Spread();
	void Rotate();
	void Squeeze();
	void Splay();

	float GetFingersDistance();
	void SetFingersCenterPos();
	float GetDistanceFromCenter(float x, float y);

	State m_CurrentState;
	int m_TouchNumer;

	bool	m_SingleTapFlag;
	ofPoint m_SingleTapPosition;

	bool m_TwoFingersFlag;
	std::array<ofPoint, 2> m_TransformBasePosition;

	float m_LastTouchOneTime;
	float m_LastTouchTwoTime;

	float m_LsatSingleTapTime;

	float m_FirstDistanceFourFingers;
	float m_LastDistanceFourFingers;
	float m_CenterX;
	float m_CenterY;

	PhotoAlbum m_PhotoAlbum;
};

#endif
