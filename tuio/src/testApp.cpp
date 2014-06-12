#include "testApp.h"
#include "Photo.h"

int TransitionTable[][INPUT_TYPE_NUMBER] =
{
	1, -1, 0, -1, -1, 1,	// 사진첩 닫힘 상태
	2, 0, 1, -1, 0, -1,		// 사진첩 열림 상태
	-1, 1, 3, 3, 1, -1,		// 사진 선택 상태
	-1, 2, 3, 3, 1, -1		// 사진 변형 상태
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
	m_TwoFingersFlag = false;
	m_TouchNumer = 0;
	
	m_FirstDistanceFourFingers = 0.0f;
	m_LastDistanceFourFingers = 0.0f;

	m_CenterX = 0.0f;
	m_CenterY = 0.0f;

	m_PhotoAlbum.SetAlbumImage("adventure_time.jpg");
	m_PhotoAlbum.SetScale(0.3f); // 너무 크다

	Photo* finn = new Photo();
	finn->SetImage("finn.png");
	m_PhotoAlbum.AddPhoto(finn);

	Photo* jake = new Photo();
	jake->SetImage("jake.png");
	m_PhotoAlbum.AddPhoto(jake);

	Photo* marceline = new Photo();
	marceline->SetImage("marceline.png");
	m_PhotoAlbum.AddPhoto(marceline);

	Photo* ice_king = new Photo();
	ice_king->SetImage("ice_king.png");
	m_PhotoAlbum.AddPhoto(ice_king);
	
	Photo* bubblegum = new Photo();
	bubblegum->SetImage("bubblegum.png");
	m_PhotoAlbum.AddPhoto(bubblegum);

	m_CurrentSelectedPhoto = nullptr;
}

//--------------------------------------------------------------
void testApp::update()
{
	if (m_SingleTapFlag)
	{
		if (ofGetElapsedTimef() - m_LsatSingleTapTime > TAP_TIME_GAP)
		{
			SingleTap();
		}
	}

	// drag 추가 할 것

	if (m_TwoFingersFlag)
	{
		// 판정
		Transform();
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:
		// 앨범을 그린다.
		m_PhotoAlbum.Display(false);
		break;
	case State::PHOTO_ALBUM_OPEN:
		// 앨범과 안에 있는 사진들의 미리보기를 그린다
		m_PhotoAlbum.Display(true);
		break;
	case State::PHOTO_SELECTED:
	case State::PHOTO_TRANSFORMED:
		// 현재 선택된 그림을 그린다.
		m_CurrentSelectedPhoto->Display();
		break;
	default:
		break;
	}
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
	// for debugging
	// printf("down : %f\n", ofGetElapsedTimef());

	m_TouchNumer = myTuio.client->getTuioCursors().size();
	m_TwoFingersFlag = false;

	switch (m_TouchNumer)
	{
	case 1:
		m_LastTouchOneTime = ofGetElapsedTimef();
		m_SingleTapPosition.x = touch.x * WINDOW_WIDTH;
		m_SingleTapPosition.y = touch.y * WINDOW_HEIGHT;
		break;
	case 2:
		m_LastTouchTwoTime = ofGetElapsedTimef();
		m_TwoFingersFlag = true;
		SetBasePoints();
		break;
	case 4:
		// 손가락 사이의 거리를 측정해서 저장
		m_FirstDistanceFourFingers = GetFingersDistance();
		m_LastDistanceFourFingers = 0.0f;
		break;
	default:
		break;
	}
}

void testApp::SetBasePoints()
{
	int i = 0;
	for ( auto *iter : myTuio.client->getTuioCursors() )
	{
		if (i >= 2)
			return;

		m_TransformBasePosition[i].x = iter->getX() * WINDOW_WIDTH;
		m_TransformBasePosition[i].y = iter->getY() * WINDOW_HEIGHT;
		++i;
	}
}

void testApp::touchUp(ofTouchEventArgs & touch)
{
	// for debugging
	// printf("up : %f\n", ofGetElapsedTimef());

	float tempX = touch.x * WINDOW_WIDTH;
	float tempY = touch.y * WINDOW_HEIGHT;

	// printf("down : %f , %f\n", tempX, tempY);

	if (m_TouchNumer == 4 && myTuio.client->getTuioCursors().size() > 4)
	{
		m_LastDistanceFourFingers += GetDistanceFromCenter(tempX, tempY);
	}

	if (myTuio.client->getTuioCursors().size() != 1)
		return;

	float currentTime = ofGetElapsedTimef();

	 // 터치를 떼면서 작동되는 것들
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
				m_SingleTapPosition.x = tempX;
				m_SingleTapPosition.y = tempY;
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
	
	m_TwoFingersFlag = false;
	m_TouchNumer = 0;
}

void testApp::touchMoved(ofTouchEventArgs & touch)
{
	float tempX = touch.x * WINDOW_WIDTH;
	float tempY = touch.y * WINDOW_HEIGHT;

	if (myTuio.client->getTuioCursors().size() == 1)
	{
		Drag(tempX, tempY);
	}
}

void testApp::SetFingersCenterPos()
{
	// 각 좌표 누적
	for ( auto *iter : myTuio.client->getTuioCursors() )
	{
		m_CenterX += (iter->getX() * WINDOW_WIDTH);
		m_CenterY += (iter->getY() * WINDOW_HEIGHT);
	}

	// 평균
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
	터치 수가 4개 이면 각각의 터치 좌표의 중심(평균)과 각각의 점들의 거리를 누적해서 저장
	현재 값을 저장된 값과 비교, 변화량 / 시간차 > 기준 이면 splay, 작으면 squeeze
	*/
	float returnVal = 0.0f;

	SetFingersCenterPos();

	// 분산
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
	Photo* selectedPhoto = nullptr;

	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:

		if (!m_PhotoAlbum.Touch(m_SingleTapPosition) )
			return;
		
		m_PhotoAlbum.Open();
		// printf("OPEN\n");
		break;

	case State::PHOTO_ALBUM_OPEN:

		// 사진이 선택되었는지 판정해서 사진 선택
		// 사진 선택 아니면 리턴
		selectedPhoto = m_PhotoAlbum.GetSelectedPhoto(m_SingleTapPosition);
		if (selectedPhoto != nullptr)
		{
			m_CurrentSelectedPhoto = selectedPhoto;
			m_CurrentSelectedPhoto->Reset();
		}
		else
		{
			return;
		}
		break;

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

		// 앨범 닫힘 상태로 돌아간다
		m_PhotoAlbum.Close();
		break;

	case State::PHOTO_SELECTED:

		// 앨범 열림 상태로 돌아간다
		m_PhotoAlbum.Open();
		break;

	case State::PHOTO_TRANSFORMED:

		// 사진 선택 상태로 돌아간다
		m_CurrentSelectedPhoto->Reset();
		break;

	case State::PHOTO_ALBUM_CLOSED:
	default:
		return;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::DOUBLE_TAP)] );
}

void testApp::Drag(float tempX, float tempY)
{
	Photo* selectedPhoto = nullptr;

	// drag
	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:

		if (m_PhotoAlbum.Touch(tempX, tempY))
			m_PhotoAlbum.SetPosition(tempX, tempY);
		else
			return;
		break;

	case State::PHOTO_ALBUM_OPEN:

		selectedPhoto = m_PhotoAlbum.GetSelectedPhoto(m_SingleTapPosition);
		if (selectedPhoto != nullptr)
			selectedPhoto->SetPosition(tempX, tempY);
		else
			return;
		break;

	case State::PHOTO_SELECTED:
	case State::PHOTO_TRANSFORMED:

		if (m_CurrentSelectedPhoto->Touch(tempX, tempY))
			m_CurrentSelectedPhoto->SetPosition(tempX, tempY);
		else
			return;
		break;

	default:
		return;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::DRAG)] );
}

void testApp::Transform()
{
	if (m_CurrentState != State::PHOTO_SELECTED &&
		m_CurrentState != State::PHOTO_TRANSFORMED)
		return;

	// 사진 변형
	int i = 0;
	for ( auto *iter : myTuio.client->getTuioCursors() )
	{
		if (i >= 2)
			return;

		m_TransformCurrentPosition[i].x = iter->getX() * WINDOW_WIDTH;
		m_TransformCurrentPosition[i].y = iter->getY() * WINDOW_HEIGHT;
		++i;
	}

	// 각도 구하기
	float originAngle = atanf(
		(m_TransformBasePosition[1].y - m_TransformBasePosition[0].y) / 
		(m_TransformBasePosition[1].x - m_TransformBasePosition[0].x)
		);
	float currentAngle = atanf(
		(m_TransformCurrentPosition[1].y - m_TransformCurrentPosition[0].y) / 
		(m_TransformCurrentPosition[1].x - m_TransformCurrentPosition[0].x)
		);

	m_CurrentSelectedPhoto->SetAngle( (currentAngle - originAngle) * 360 / PI );

	// 스케일 구하기
	float originDistance = sqrt( 
		pow(m_TransformBasePosition[0].x - m_TransformBasePosition[1].x, 2.0f) + 
		pow(m_TransformBasePosition[0].y - m_TransformBasePosition[1].y, 2.0f) 
		);
	float currentDistance = sqrt( 
		pow(m_TransformCurrentPosition[0].x - m_TransformCurrentPosition[1].x, 2.0f) + 
		pow(m_TransformCurrentPosition[0].y - m_TransformCurrentPosition[1].y, 2.0f) 
		);

	if (currentDistance / originDistance < MINIMUM_SCALE)
		m_CurrentSelectedPhoto->SetScale( MINIMUM_SCALE );
	else
		m_CurrentSelectedPhoto->SetScale( currentDistance / originDistance );

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::TRANSFORM)] );
}

void testApp::Squeeze()
{
	printf("squeeze\n");

	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_OPEN:
		
		// 앨범 닫힌 상태로 돌아감
		m_PhotoAlbum.Close();
		break;

	case State::PHOTO_SELECTED:
		
		// 앨범 열림 상태로 돌아감
		m_PhotoAlbum.Open();
		break;

	case State::PHOTO_TRANSFORMED:

		// 앨범 열린 상태로 돌아감
		m_PhotoAlbum.Open();
		break;

	default:
		return;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::SQUEEZE)] );
}

void testApp::Splay()
{
	printf("splay\n");

	switch (m_CurrentState)
	{
	case State::PHOTO_ALBUM_CLOSED:

		// 앨범 오픈!
		m_PhotoAlbum.Open();
		break;

	default:
		return;
	}

	m_CurrentState = static_cast<State>( TransitionTable[static_cast<int>(m_CurrentState)][static_cast<int>(InputType::SPLAY)] );
}
