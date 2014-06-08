#pragma once

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;

const int RAIN_BLCOK_SIZE = 20;
const int TEXT_BLOCK_NUMBER = WINDOW_WIDTH / RAIN_BLCOK_SIZE;

const float TEXT_BLOCK_SPEED = 30.0f;

namespace CollisionCheck
{
	static bool IsCollide()
	{
		return true;
	}
}