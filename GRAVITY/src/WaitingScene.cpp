#include "WaitingScene.h"
#include "ofMain.h"

WaitingScene::WaitingScene(void)
{
	m_IsTransition = false;
	m_TransitionKey = 0.0f;
	m_CurrentState = NOTHING;
	m_TimeStampPrev = GetTickCount();
}


WaitingScene::~WaitingScene(void)
{
}

void WaitingScene::Display()
{
	if ( m_IsTransition )
	{
		// 화면 전환 재생
		ofBackground( 0, 0, 0 );

		// 재생 완료
		unsigned long currentTime = GetTickCount();
		float dt = (currentTime - m_TimeStampPrev) / 1000.0f;
		m_TimeStampPrev = currentTime;

		m_TransitionKey += dt;
		if ( m_TransitionKey >= SCENE_TRANSITION_PALY_TIME )
			m_IsTransition = false;
	}
	else if ( m_CurrentState == WAITING )
	{
		// 대기 화면 보여준다
		ofBackground( 255, 255, 255 );
	}
}

void WaitingScene::ChangeState( WaitingSceneState state )
{
	m_CurrentState = state;

	// transition을 시작한다
	m_IsTransition = true;
	m_TransitionKey = 0.0f;
}
