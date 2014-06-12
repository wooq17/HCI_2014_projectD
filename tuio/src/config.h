#pragma once

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1200

const float PREVIEW_IMG_SIZE = 150.0f;
const float PREVIEW_DISTANCE = 400.0f;

const int INPUT_TYPE_NUMBER = 6;

const float TAP_TIME_GAP = 0.8f;

const float MINIMUM_SCALE = 0.2f;

enum class InputType : int
{
	SINGLE_TAP,
	DOUBLE_TAP,
	DRAG,
	TRANSFORM,
	SQUEEZE,
	SPLAY,
};

enum class State : int
{
	PHOTO_ALBUM_CLOSED,	// 0
	PHOTO_ALBUM_OPEN,	// 1
	PHOTO_SELECTED,		// 2
	PHOTO_TRANSFORMED,	// 3
};

