#include "testApp.h"

#define IMAGE_SIZE_X 1620
#define IMAGE_SIZE_Y 1080

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);

	if ( !m_Image.loadImage("test_img.jpg"))
	{
		ofLog(OF_LOG_ERROR, "cannot open the image");
	}

	m_ImageSizeWeight = 1.0f;

	m_MousePrevX = ofGetWidth() / 2;
	m_MousePrevY = ofGetHeight() / 2;

	m_ImagePositionX = m_MousePrevX;
	m_ImagePositionY = m_MousePrevY;

	m_MouseCurrX = m_MousePrevX;
	m_MouseCurrY = m_MousePrevY;

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
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);

	// hand event를 담당하는 리스너 등록
	ofAddListener(openNIDevice.handEvent, this, &testApp::headEvent);
	ofAddListener(openNIDevice.handEvent, this, &testApp::headEvent);

	m_IsWaiting = true;

	m_IsTrackingPrev = false;
	m_IsTrackingCurr = false;
}

//--------------------------------------------------------------
void testApp::update(){
	// openNI
    openNIDevice.update();
	m_IsTrackingCurr = openNIDevice.getNumTrackedUsers() > 0 ? true : false;

	// isTracking
	if ( !m_IsTrackingPrev && m_IsTrackingCurr )
	{
		// detect
		m_IsWaiting = false;

		// start the GRAVITY
	}
	else if ( m_IsTrackingPrev && !m_IsTrackingCurr )
	{
		// lost
		m_IsWaiting = true;

		// stop the GRAVITY
	}
	
	m_IsTrackingPrev = m_IsTrackingCurr;

	// draw image


	// image transform
	m_ImageSizeWeight += ( ( m_MouseCurrY - m_MousePrevY ) / 5000.0f );
	m_MousePrevY = m_MouseCurrY;

	m_ImagePositionX += ( ( m_MouseCurrX - m_MousePrevX ) / 20.0f );
	m_MousePrevX = m_MouseCurrX;
}

//--------------------------------------------------------------
void testApp::draw(){
	/*
	ofSetColor(255, 255, 255);
    
    ofPushMatrix();
    // draw debug (ie., image, depth, skeleton)
    //openNIDevice.drawDebug();
	openNIDevice.drawDepth();
    ofPopMatrix();
    
    ofPushMatrix();
    // get number of current hands
    int numHands = openNIDevice.getNumTrackedHands();
    
    // iterate through users
    for (int i = 0; i < numHands; i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
        // do something with the positions like:
        
        // draw a rect at the position (don't confuse this with the debug draw which shows circles!!)
        ofSetColor(255,0,0);
        ofRect(handPosition.x, handPosition.y, 10, 10);
        
    }
    ofPopMatrix();
    
    // draw some info regarding frame counts etc
	ofSetColor(0, 255, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
    
	verdana.drawString(msg, 20, openNIDevice.getNumDevices() * 480 - 20);
	*/
	m_Image.draw(
		m_ImagePositionX - (IMAGE_SIZE_X * m_ImageSizeWeight / 2), 
		m_ImagePositionY - (IMAGE_SIZE_Y * m_ImageSizeWeight / 2), 
		IMAGE_SIZE_X * m_ImageSizeWeight, 
		IMAGE_SIZE_Y * m_ImageSizeWeight 
		);
}

//--------------------------------------------------------------
void testApp::headEvent(ofxOpenNIHandEvent & event){
    // show hand event messages in the console
	// 머리 인식하면 씬 시작
	// 이동하면 화면 움직임
    ofLogNotice() << getHandStatusAsString(event.handStatus) << "for hand" << event.id << "from device" << event.deviceID;
}

void testApp::userEvent(ofxOpenNIUserEvent& event)
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
	m_MouseCurrX = x;
	m_MouseCurrY = y;
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