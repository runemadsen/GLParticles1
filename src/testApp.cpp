#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground(20, 20, 20);
	ofSetVerticalSync(true);
	
	particleSystem.loadTexture("particleGrid2.png", 2, 2);
	particleSystem.init();
	
}

//--------------------------------------------------------------
void testApp::update() 
{	
	particleSystem.update();
}

//--------------------------------------------------------------
void testApp::draw() 
{
	particleSystem.render();
	
	// draw the texture
	ofEnableAlphaBlending();
	ofSetColor(0xffffff);
	particleSystem.texture.draw(0, 50);
	ofDisableAlphaBlending();
	
	
	ofSetColor(255, 255, 255);
	string info = "FPS: "+ofToString(ofGetFrameRate());
	info += "\nTotal Particles: "+ofToString(MAX_PARTICLES);
	
	ofDrawBitmapString(info, 20, 20);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) 
{	
	if(key == 'f') ofToggleFullscreen();
	
	if(key == ' ') {
		ofImage img;
		img.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		string uid = ofToString(ofGetDay())+ofToString(ofGetYear())+ofToString(ofGetSeconds())+ofToString(ofGetHours());
		img.saveImage("OMS_"+uid+".tiff");
	}
}


void testApp::keyReleased  (int key) {
	
}

void testApp::mouseMoved(int x, int y ){
	
}

void testApp::mouseDragged(int x, int y, int button) {

}

void testApp::mousePressed(int x, int y, int button) {
	
	particleSystem.setFlyAway();

}

void testApp::mouseReleased(int x, int y, int button) {
	
}

void testApp::windowResized(int w, int h) {
	
}
