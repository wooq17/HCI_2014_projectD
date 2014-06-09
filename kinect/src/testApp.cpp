#include "testApp.h"

#define IMAGE_SIZE_X 1620
#define IMAGE_SIZE_Y 1080

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(24);

    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
	openNIDevice.setUseDepthRawPixels(true);
    
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
	}

	m_DepthMap.fill(false);

	// hand event를 담당하는 리스너 등록
	// ofAddListener(openNIDevice.handEvent, this, &testApp::handEvent);

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
	ofShortPixels depthMap = openNIDevice.getDepthRawPixels();

	int depthMapIdx = 0;
	for (unsigned int i = 0; i < KINECT_RESOLUTION_HEIGHT; i += STEP)
	{
		for (unsigned int j = 0; j < KINECT_RESOLUTION_WIDTH; j += STEP)
		{
			depthMapIdx = j + (KINECT_RESOLUTION_WIDTH * i);
			unsigned short tempShort = depthMap[depthMapIdx];

			//if ( depthMap[depthMapIdx] > 0 && depthMap[depthMapIdx] < 800)
			if ( depthMap[depthMapIdx] > 0 && depthMap[depthMapIdx] < 2500)
			{
				// 사람
				m_DepthMap[depthMapIdx] = true;
			}
			else
			{
				// 그냥 배경
				m_DepthMap[depthMapIdx] = false;
			}
		}
	}
	
	// text blocks
	for (int i = 0; i < TEXT_BLOCK_NUMBER; ++i)
	{
		m_TextBlock[i].Update(dt);
		
		bool isEnd = false;

		// text block position
		int posX = i * TEXT_BLOCK_SIZE / 2;
		int posY = static_cast<int>(m_TextBlock[i].GetPosition()) / 2;

		// text block 안의 depthMap 상태를 확인
		// 높은 곳부터 확인해서 만약 true가 있으면 text block을 그 위치로 이동
		for (int depthMapY = posY - (TEXT_BLOCK_SIZE / 2); depthMapY < posY; ++depthMapY)
		{
			if (depthMapY >= 400 || depthMapY < 0)
				continue;

			for (int depthMapX = posX; depthMapX < posX + (TEXT_BLOCK_SIZE / 2); ++depthMapX)
			{
				if (depthMapX >= 640 || depthMapX < 0)
					continue;

				depthMapIdx = (depthMapY * KINECT_RESOLUTION_WIDTH) + depthMapX;

				if ( m_DepthMap[depthMapIdx] )
				{
					m_TextBlock[i].SetPosition(depthMapY * 2);
					isEnd = true;
					break;
				}
			}

			if (isEnd)
				break;
		}

		m_TextBlock[i].BeTouched(false);

		int numHands = openNIDevice.getNumTrackedHands();
		// iterate through users
		for (int j = 0; j < numHands; j++)
		{
			// get a reference to this user
			ofxOpenNIHand & hand = openNIDevice.getTrackedHand(j);
        
			// get hand position
			ofPoint & handPosition = hand.getPosition();

			if ( CollisionCheck::IsCollide( ( i + 0.5f ) * TEXT_BLOCK_SIZE, m_TextBlock[i].GetPosition(), handPosition.x * 2, handPosition.y * 2 ) )
			{
				m_TextBlock[i].BeTouched(true);
				break;
			}
		}
	}
	
	// hand 위치와 비교해서 컬러 변화
}

//--------------------------------------------------------------
void testApp::draw(){
	// image
	ofSetColor(255, 255, 255);
	openNIDevice.drawImage(0.0f, 0.0f, KINECT_RESOLUTION_WIDTH * 2, KINECT_RESOLUTION_HEIGHT * 2);

	// text
	for (int i = 0; i < TEXT_BLOCK_NUMBER; ++i)
	{
		ofSetColor(m_TextBlock[i].GetColor());
		m_Text.drawString(m_TextBlock[i].GetText(), i * TEXT_BLOCK_SIZE, m_TextBlock[i].GetPosition() - TEXT_BLOCK_SIZE);
	}

	// for debugging
	/*
	int depthMapIdx = 0;
	for (unsigned int i = 0; i < KINECT_RESOLUTION_HEIGHT; i += STEP)
	{
		for (unsigned int j = 0; j < KINECT_RESOLUTION_WIDTH; j += STEP)
		{
			depthMapIdx = j + (640 * i);
			
			if (m_DepthMap[depthMapIdx])
			{
				// 사람 인식한 영역
				ofSetColor(150, 0, 0);
				ofRect(j * 2, i * 2, STEP, STEP);
			}
		}
	}
	*/
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