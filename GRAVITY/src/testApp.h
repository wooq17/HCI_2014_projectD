#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include <array>

#define MAX_DEVICES 2
#define MAX_NUMBER_OF_HAND 4

class testApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

private:
    void headEvent(ofxOpenNIHandEvent& event);
	void userEvent(ofxOpenNIUserEvent& event);
    
	ofxOpenNI openNIDevice;
    ofTrueTypeFont verdana;
    ofImage m_Image;
    
	bool m_IsTrackingPrev;
	bool m_IsTrackingCurr;

	bool m_IsWaiting;

	int m_MousePrevX;
	int m_MousePrevY;

	int m_MouseCurrX;
	int m_MouseCurrY;

	float m_ImagePositionX;
	float m_ImagePositionY;

	float m_ImageSizeWeight;
};

#endif
