#include "testApp.h"
#include <math.h>
//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);

	// openNI
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
	openNIDevice.setUseDepthRawPixels(true);
	openNIDevice.addUserGenerator();
	openNIDevice.setSkeletonProfile(XN_SKEL_PROFILE_UPPER);
    
    // setup the hand generator
    openNIDevice.addHandsGenerator();
    
    // add all focus gestures (ie., wave, click, raise arm)
    openNIDevice.addAllHandFocusGestures();
    
    openNIDevice.setMaxNumHands(MAX_NUMBER_OF_HAND);
    
    openNIDevice.start();
    
	m_Text.loadFont(ofToDataPath("verdana.ttf"), 10);

	// create image
	if ( !m_Image.loadImage("test_img.jpg"))
	{
		ofLog(OF_LOG_ERROR, "cannot open the image");
	}
	IMAGE_SIZE_X = m_Image.getWidth();
	IMAGE_SIZE_Y = m_Image.getHeight();

	m_CurrentEyePosition = ofVec3f( 0.0f );

	m_IsTrackingCurr = false;
	m_IsTrackingPrev = false;

	m_WaitingScene.ChangeState( WAITING );

	m_ImageCenterPositionX = SCREEN_SIZE_X / 2;
	m_ImageCenterPositionY = SCREEN_SIZE_Y / 2;
	m_ImageScale = 1.0f;

	m_ImageCenterPositionXR = m_ImageCenterPositionX;
	m_ImageCenterPositionYR = m_ImageCenterPositionY;
	m_ImageScaleR = m_ImageScale;

	m_ScreenKinectResolutionRatio = static_cast<float>( SCREEN_SIZE_X ) / KINECT_RESOLUTION_WIDTH; // 가로 폭이 훨씬 크니까 일단은 세로축 비율은 고려 안 함

	m_ScaleTransformConstant = ( ( KINECT_SENSOR_FAR / ( 5 * IMAGE_SCALE_MIN ) ) - KINECT_SENSOR_NEAR ) / ( 1 - ( 1 / ( 5 * IMAGE_SCALE_MIN) ) );
	m_ScaleTransformGuide = ( KINECT_SENSOR_NEAR * IMAGE_SIZE_X ) / ( KINECT_SENSOR_NEAR + m_ScaleTransformConstant );
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
		m_WaitingScene.ChangeState( HIDE );
	}
	else if ( m_IsTrackingPrev && !m_IsTrackingCurr )
	{
		// lost
		m_WaitingScene.ChangeState( WAITING );
	}
	
	m_IsTrackingPrev = m_IsTrackingCurr;

	// 방어 코드
	if ( !m_IsTrackingCurr )
	{
		return;
	}

	// set eye position
	ofxOpenNIUser &user = openNIDevice.getTrackedUser( 0 );

	ofxOpenNIJoint head = user.getJoint( JOINT_HEAD );
	ofPoint point = head.getProjectivePosition();
	
	ofShortPixels depthMap = openNIDevice.getDepthRawPixels();
	int depthMapIdx = static_cast<int>( point.x ) + ( KINECT_RESOLUTION_WIDTH * static_cast<int>( point.y ) );

	m_CurrentEyePosition.x = point.x;
	// m_CurrentEyePosition.y = point.y;
	m_CurrentEyePosition.z = depthMap[depthMapIdx];
	m_CurrentEyePosition.y = point.y + ( ( m_CurrentEyePosition.z - EYE_HEIGHT_CORRECTION_ZEROING_POINT ) * EYE_HEIGHT_CORRECTION ) - 20.0f;

	// image center position
	if ( m_CurrentEyePosition.z == 0.0f )
		return;

	m_ImageCenterPositionX = ( SCREEN_SIZE_X / 2 ) - ( ( ( SCREEN_SIZE_X / 2 ) - ( m_CurrentEyePosition.x * m_ScreenKinectResolutionRatio ) ) * m_ScaleTransformConstant ) / m_CurrentEyePosition.z;
	m_ImageCenterPositionY = ( SCREEN_SIZE_Y / 2 ) - ( ( ( SCREEN_SIZE_Y / 2 ) - ( m_CurrentEyePosition.y * m_ScreenKinectResolutionRatio ) ) * m_ScaleTransformConstant ) / m_CurrentEyePosition.z;
	// m_ImageCenterPositionY = ( m_CurrentEyePosition.y * m_ScreenKinectResolutionRatio ) - ( EYE_HEIGHT_CORRECTION * ( EYE_HEIGHT_CORRECTION_ZEROING_POINT - m_CurrentEyePosition.z ) ) - 100.0f;
	// m_ImageCenterPositionY = ( m_CurrentEyePosition.y * m_ScreenKinectResolutionRatio ) + ( ( m_CurrentEyePosition.y - EYE_HEIGHT_CORRECTION_ZEROING_POINT ) * EYE_HEIGHT_CORRECTION );

	// image scale
	// m_ImageScale = IMAGE_SIZE_X / ( ( m_CurrentEyePosition.z + m_ScaleTransformConstant ) * m_ScaleTransformGuide / m_CurrentEyePosition.z );
	// m_ImageScale = pow( ( m_CurrentEyePosition.z - EYE_HEIGHT_CORRECTION_ZEROING_POINT ), 2.0f ) / ( IMAGE_SCALE_MIN * pow( EYE_HEIGHT_CORRECTION_ZEROING_POINT, 2.0f ) ) + IMAGE_SCALE_MIN;
	m_ImageScale = 1 / ( ( ( 1.0f - IMAGE_SCALE_MIN ) / pow( KINECT_SENSOR_FAR, 2.0f ) ) * pow( m_CurrentEyePosition.z - KINECT_SENSOR_FAR, 2.0f ) + IMAGE_SCALE_MIN );
}

//--------------------------------------------------------------
void testApp::draw(){
	// ofBackground(150);

	// image
	ofSetColor( 255, 255, 255 );

	if ( m_ImageCenterPositionX != 0.0f )
		m_ImageCenterPositionXR = ( m_ImageCenterPositionXR * EASING_WEIGHT ) + ( m_ImageCenterPositionX * ( 1.0f - EASING_WEIGHT ) );
	
	if ( m_ImageCenterPositionY != 0.0f )
		m_ImageCenterPositionYR = ( m_ImageCenterPositionYR * EASING_WEIGHT ) + ( m_ImageCenterPositionY * ( 1.0f - EASING_WEIGHT ) );

	if ( m_ImageScale != 0.0f && m_ImageScale < 2.0f )
		m_ImageScaleR = ( m_ImageScaleR * EASING_WEIGHT ) + ( m_ImageScale * ( 1.0f - EASING_WEIGHT ) );

	m_Image.draw(
		m_ImageCenterPositionXR - (IMAGE_SIZE_X * m_ImageScaleR / 2), 
		m_ImageCenterPositionYR - (IMAGE_SIZE_Y * m_ImageScaleR / 2), 
		IMAGE_SIZE_X * m_ImageScaleR, 
		IMAGE_SIZE_Y * m_ImageScaleR 
		);

	// openNIDevice.drawImage(0.0f, 0.0f, KINECT_RESOLUTION_WIDTH * m_ScreenKinectResolutionRatio, KINECT_RESOLUTION_HEIGHT * m_ScreenKinectResolutionRatio);
	// ofSetColor( 0, 0, 100 );
	// ofRect(m_ImageCenterPositionX - 3, m_ImageCenterPositionY - 3, 6, 6);

	// waiting scene
	// m_WaitingScene.Display();

	// DrawDebugInfo();
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

void testApp::DrawDebugInfo()
{
	std::string debugInfo = "";

	ofSetColor( 200, 0, 200 );
	debugInfo.append( "CurrentEyePosition : " );
	debugInfo.append( std::to_string( m_CurrentEyePosition.x ) );
	debugInfo.append( ", " );
	debugInfo.append( std::to_string( m_CurrentEyePosition.y ) );
	debugInfo.append( ", " );
	debugInfo.append( std::to_string( m_CurrentEyePosition.z ) );
	m_Text.drawString( debugInfo, 20.0f, 20.0f );
	debugInfo.clear();

	debugInfo.append( "ImageCenterPosition : " );
	debugInfo.append( std::to_string( m_ImageCenterPositionX ) );
	debugInfo.append( ", " );
	debugInfo.append( std::to_string( m_ImageCenterPositionY ) );
	m_Text.drawString( debugInfo, 20.0f, 45.0f );
	debugInfo.clear();

	debugInfo.append( "ImageScale : " );
	debugInfo.append( std::to_string( m_ImageScale ) );
	m_Text.drawString( debugInfo, 20.0f, 60.0f );
	debugInfo.clear();

	debugInfo.append( "ScaleTransformConstant : " );
	debugInfo.append( std::to_string( m_ScaleTransformConstant ) );
	m_Text.drawString( debugInfo, 20.0f, 75.0f );
	debugInfo.clear();

	debugInfo.append( "ScaleTransformGuide : " );
	debugInfo.append( std::to_string( m_ScaleTransformGuide ) );
	m_Text.drawString( debugInfo, 20.0f, 90.0f );
	debugInfo.clear();

	debugInfo.append( "ScreenKinectResolutionRatio : " );
	debugInfo.append( std::to_string( m_ScreenKinectResolutionRatio ) );
	m_Text.drawString( debugInfo, 20.0f, 105.0f );
	debugInfo.clear();

	debugInfo.append( "ImageSize : " );
	debugInfo.append( std::to_string( IMAGE_SIZE_X ) );
	debugInfo.append( ", " );
	debugInfo.append( std::to_string( IMAGE_SIZE_Y ) );
	m_Text.drawString( debugInfo, 20.0f, 120.0f );
	debugInfo.clear();
}