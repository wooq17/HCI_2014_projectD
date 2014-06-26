#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include <array>

#include "config.h"
#include "WaitingScene.h"

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

	void DrawDebugInfo();

	int IMAGE_SIZE_X;
	int IMAGE_SIZE_Y;

private:
    
	ofxOpenNI openNIDevice;

    ofImage			m_Image;
	WaitingScene	m_WaitingScene;

	// main logic
	ofVec3f m_CurrentEyePosition; // x, y, z

	bool	m_IsTrackingCurr;
	bool	m_IsTrackingPrev;

	float	m_ImageCenterPositionX;
	float	m_ImageCenterPositionY;
	float	m_ImageScale;

	float	m_ImageCenterPositionXR;
	float	m_ImageCenterPositionYR;
	float	m_ImageScaleR;

	float	m_ScaleTransformConstant;	// C
	float	m_ScaleTransformGuide;		// h

	float	m_ScreenKinectResolutionRatio;

	// debug
	ofTrueTypeFont m_Text;
};

#endif
