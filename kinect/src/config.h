#pragma once

#include <math.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;

const int KINECT_RESOLUTION_WIDTH = 640;
const int KINECT_RESOLUTION_HEIGHT = 400;

const int TEXT_BLOCK_SIZE = 20;
const int TEXT_BLOCK_NUMBER = WINDOW_WIDTH / TEXT_BLOCK_SIZE;

const float TEXT_BLOCK_SPEED = 30.0f;

const int STEP = 4;

const float COLLIDE_DISTANCE = 40.0f;

namespace CollisionCheck
{
	static bool IsCollide(float x1, float y1, float x2, float y2)
	{
		float distance = sqrt( pow(x2 - x1, 2.0f) + pow(y2 - y1, 2.0f) );
		return distance < COLLIDE_DISTANCE;
	}
}