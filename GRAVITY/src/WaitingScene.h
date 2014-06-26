#pragma once

#include "config.h"

class WaitingScene
{
public:
	WaitingScene(void);
	~WaitingScene(void);

	void Display();
	void ChangeState( WaitingSceneState state );

private:
	bool	m_IsTransition;
	float	m_TransitionKey;

	bool	m_IsDisplayed;

	WaitingSceneState m_CurrentState;

	unsigned long m_TimeStampPrev;
};

