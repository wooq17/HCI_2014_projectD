#pragma once

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

const float PREVIEW_IMG_SIZE = 50.0f;
const float PREVIEW_DISTANCE = 200.0f;

const int INPUT_TYPE_NUMBER = 8;

const float TAP_TIME_GAP = 0.8f;

enum class InputType : int
{
	SINGLE_TAP,
	DOUBLE_TAP,
	DRAG,
	PINCH,
	SPREAD,
	ROTATE,
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
