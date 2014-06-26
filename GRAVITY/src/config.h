#pragma once

const int SCREEN_SIZE_X = 5760;
const int SCREEN_SIZE_Y = 2160;

const int KINECT_RESOLUTION_WIDTH = 640;
const int KINECT_RESOLUTION_HEIGHT = 480;

const float IMAGE_SCALE_MIN = 0.65f;

const unsigned short KINECT_SENSOR_NEAR = 800;
const unsigned short KINECT_SENSOR_FAR = 4000;

const float SCENE_TRANSITION_PALY_TIME = 5.0f;

enum WaitingSceneState
{
	NOTHING,
	WAITING,
	HIDE,
};

const float EYE_HEIGHT_CORRECTION = 5.0f / 77.0f;
const float EYE_HEIGHT_CORRECTION_ZEROING_POINT = 3308.0f;

const float EASING_WEIGHT = 0.9f;
