
#include "testApp.h"
#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "config.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, SCREEN_SIZE_X, SCREEN_SIZE_Y, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
