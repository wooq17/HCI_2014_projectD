#include "testApp.h"
#include <array>

int TransitionTable[][INPUT_TYPE_NUMBER] =
{
	1, -1, 0, -1, -1, -1, -1, 1,	// ����ø ���� ����
	2, 0, 1, -1, -1, -1, 0, -1,		// ����ø ���� ����
	-1, 1, 3, 3, 3, 3, 1, -1,		// ���� ���� ����
	-1, 2, 3, 3, 3, 3, 1, -1		// ���� ���� ����
};

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(100, 100, 100);
	ofSetVerticalSync(true);
	
	//Connect to Port
	myTuio.connect(3333);
	
	//Assign Global TUIO Callback Functions
	ofAddListener(ofEvents().touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents().touchUp, this, &testApp::touchUp);
	ofAddListener(ofEvents().touchMoved, this, &testApp::touchMoved);

	m_CurrentState = State::PHOTO_ALBUM_CLOSED;

	m_SingleTapFlag = false;
	m_TouchNumer = 0;
	
	m_FirstDistanceFourFingers = 0.0f;
	m_LastDistanceFourFingers = 0.0f;

	m_CenterX = 0.0f;
	m_CenterY = 0.0f;
}

//--------------------------------------------------------------
void testApp::update()
{
	// ��ġ ���� �����ϸ鼭 �۵��ϴ� ��� ó��
	/*
	DRAG,
		

	PINCH,
	SPREAD,
	ROTATE,
	*/

	if (m_SingleTapFlag)
	{
		if (ofGetElapsedTimef() - m_LsatSingleTapTime > TAP_TIME_GAP)
		{
			SingleTap();
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	
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


void testApp::touchDown(ofTouchEventArgs & touch)
{
	printf("down : %f\n", ofGetElapsedTimef());
	m_TouchNumer = myTuio.client->getTuioCursors().size();

	switch (m_TouchNumer)
	{
	case 1:
		m_LastTouchOneTime = ofGetElapsedTimef();
		break;
	case 2:
		m_LastTouchTwoTime = ofGetElapsedTimef();
		break;
	case 4:
		// �հ��� ������ �Ÿ��� �����ؼ� ����
		m_FirstDistanceFourFingers = GetFingersDistance();
		m_LastDistanceFourFingers = 0.0f;
		break;
	default:
		break;
	}
}

void testApp::touchUp(ofTouchEventArgs & touch)
{
	printf("up : %f\n", ofGetElapsedTimef());

	if (m_TouchNumer == 4)
	{
		m_LastDistanceFourFingers += GetDistanceFromCenter(touch.x, touch.y);
	}

	if (myTuio.client->getTuioCursors().size() != 1)
		return;

	float currentTime = ofGetElapsedTimef();

	 // ��ġ�� ���鼭 �۵��Ǵ� �͵�
	switch (m_TouchNumer)
	{
	case 1:
		// DOUBLE_TAP,
		if (currentTime - m_LastTouchOneTime < TAP_TIME_GAP)
		{
			if (currentTime - m_LsatSingleTapTime < TAP_TIME_GAP)
			{	
				// DOUBLE_TAP,
				DoubleTap();
				currentTime = 0.0f;
			}
			else
			{
				m_SingleTapFlag = true;
				m_LsatSingleTapTime = currentTime;
			}
		}
		break;
	case 4:
		if (m_LastDistanceFourFingers > m_FirstDistanceFourFingers)
			Splay();
		else
			Squeeze();

		m_CenterX = 0.0f;
		m_CenterY = 0.0f;
		break;
	default:
		break;
	}

	m_TouchNumer = 0;
}

void testApp::touchMoved(ofTouchEventArgs & touch){
	
}

void testApp::TouchOne()
{
}

void testApp::TouchTwo()
{
}

void testApp::SetFingersCenterPos()
{
	// �� ��ǥ ����
	for ( auto *iter : myTuio.client->getTuioCursors() )
	{
		m_CenterX += iter->getX();
		m_CenterY += iter->getY();
	}

	// ���
	m_CenterX /= 4;
	m_CenterY /= 4;
}


float testApp::GetDistanceFromCenter(float x, float y)
{
	return ( pow( ( m_CenterX - x ), 2.0f) + pow( ( m_CenterY - y ), 2.0f) );
}

float testApp::GetFingersDistance()
{
	/*
	��ġ ���� 4�� �̸� ������ ��ġ ��ǥ�� �߽�(���)�� ������ ������ �Ÿ��� �����ؼ� ����
	���� ���� ����� ���� ��, ��ȭ�� / �ð��� > ���� �̸� splay, ������ squeeze
	*/
	float returnVal = 0.0f;

	SetFingersCenterPos();

	// �л�
	for ( auto *iter : myTuio.client->getTuioCursors() )
	{
		returnVal += GetDistanceFromCenter(iter->getX(), iter->getY());
	}
	
	return returnVal;
}

void testApp::SingleTap()
{
	printf("single tap\n");
	m_SingleTapFlag = false;

	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:
		// ����ø�� Ŭ���� ������ Ȯ���ؼ� ����ø ����
		// Ŭ���� �� �ƴϸ� ����
		break;
	case State::PHOTO_ALBUM_OPEN:
		// ������ ���õǾ����� �����ؼ� ���� ����
		// ���� ���� �ƴϸ� ����
		break;
	case State::PHOTO_SELECTED:
	case State::PHOTO_TRANSFORMED:
	default:
		return;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::SINGLE_TAP)] );
}

void testApp::DoubleTap()
{
	printf("double tap\n");
	m_SingleTapFlag = false;

	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_OPEN:
		// �ٹ� ���� ���·� ���ư���
		break;
	case State::PHOTO_SELECTED:
		// �ٹ� ���� ���·� ���ư���
		break;
	case State::PHOTO_TRANSFORMED:
		// ���� ���� ���·� ���ư���
		break;
	case State::PHOTO_ALBUM_CLOSED:
	default:
		return;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::DOUBLE_TAP)] );
}

void testApp::Drag()
{
	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:
		// ����
		break;
	case State::PHOTO_ALBUM_OPEN:
		// ���� ���õǾ� ������ �� ���� �̵�
		// �ƴϸ� ����
		break;
	case State::PHOTO_SELECTED:
		// ���� �̵�
		break;
	case State::PHOTO_TRANSFORMED:
		// ���� �̵�
		break;
	default:
		break;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::DRAG)] );
}

void testApp::Pinch()
{
	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:
		// ����
		break;
	case State::PHOTO_ALBUM_OPEN:
		// ����
		break;
	case State::PHOTO_SELECTED:
		// ���� ���
		break;
	case State::PHOTO_TRANSFORMED:
		// ���� ���
		break;
	default:
		break;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::PINCH)] );
}

void testApp::Spread()
{
	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:
		// ����
		break;
	case State::PHOTO_ALBUM_OPEN:
		// ����
		break;
	case State::PHOTO_SELECTED:
		// ���� Ȯ��
		break;
	case State::PHOTO_TRANSFORMED:
		// ���� Ȯ��
		break;
	default:
		break;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::SPREAD)] );
}

void testApp::Rotate()
{
	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:
		// ����
		break;
	case State::PHOTO_ALBUM_OPEN:
		// ����
		break;
	case State::PHOTO_SELECTED:
		// ���� ȸ��
		break;
	case State::PHOTO_TRANSFORMED:
		// ���� ȸ��
		break;
	default:
		break;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::ROTATE)] );
}

void testApp::Squeeze()
{
	printf("squeeze\n");

	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:
		// ����
		break;
	case State::PHOTO_ALBUM_OPEN:
		// �ٹ� ���� ���·� ���ư�
		break;
	case State::PHOTO_SELECTED:
		// �ٹ� ���� ���·� ���ư�
		break;
	case State::PHOTO_TRANSFORMED:
		// �ٹ� ���� ���·� ���ư�
		break;
	default:
		break;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::SQUEEZE)] );
}

void testApp::Splay()
{
	printf("splay\n");

	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:
		// �ٹ� ����!
		break;
	case State::PHOTO_ALBUM_OPEN:
		// ����
		break;
	case State::PHOTO_SELECTED:
		// ����
		break;
	case State::PHOTO_TRANSFORMED:
		// ����
		break;
	default:
		break;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::SPLAY)] );
}
