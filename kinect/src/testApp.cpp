#include "testApp.h"

#define IMAGE_SIZE_X 1620
#define IMAGE_SIZE_Y 1080

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    
    // setup the hand generator
    openNIDevice.addHandsGenerator();
    
    // add all focus gestures (ie., wave, click, raise arm)
    openNIDevice.addAllHandFocusGestures();
    
    openNIDevice.setMaxNumHands(MAX_NUMBER_OF_HAND);
    
    openNIDevice.start();
    
    m_Text.loadFont(ofToDataPath("verdana.ttf"), 18);

	for (int i = 0; i < TEXT_BLOCK_NUMBER; ++i)
	{
		m_TextBlock[i].Init('a');
		m_BodyEdge[i] = WINDOW_HEIGHT;
	}
	// hand event를 담당하는 리스너 등록
	ofAddListener(openNIDevice.handEvent, this, &testApp::handEvent);

	m_TimeStampPrev = GetTickCount();
}

//--------------------------------------------------------------
void testApp::update(){
	// openNI
    openNIDevice.update();

	DWORD currentTime = GetTickCount();
	float dt = (currentTime - m_TimeStampPrev) / 1000.0f;
	m_TimeStampPrev = currentTime;

	// depth map을 이용해서 on / off 설정

	// text blocks
	for (int i = 0; i < TEXT_BLOCK_NUMBER; ++i)
	{
		m_TextBlock[i].Update(dt);
		
		// 위치 보정
		if (m_TextBlock[i].GetPosition() > m_BodyEdge[i]
		&& m_TextBlock[i].GetPosition() - RAIN_BLCOK_SIZE < m_BodyEdge[i])
			m_TextBlock[i].SetPosition(m_BodyEdge[i]);
	}

	// hand 위치와 비교해서 컬러 변화
}

//--------------------------------------------------------------
void testApp::draw(){
	for (int i = 0; i < TEXT_BLOCK_NUMBER; ++i)
	{
		m_Text.drawString(m_TextBlock[i].GetText(), i * RAIN_BLCOK_SIZE, m_TextBlock[i].GetPosition());
	}
}

//--------------------------------------------------------------
void testApp::handEvent(ofxOpenNIUserEvent& event)
{
	// user event
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}