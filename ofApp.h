#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	void setRingToMesh(ofMesh& face_target, ofMesh& line_target, glm::vec3 location, float radius, float width, float height, int deg_start, int deg_end);

	ofEasyCam cam;

	ofSoundPlayer sound; // ƒŠƒtƒ@ƒŒƒ“ƒX https://openframeworks.cc/documentation/sound/ofSoundPlayer/
	vector<float> fft_value;
	vector<float> fft_smoothed;
	vector<float> deg_list;
	int n_bands_to_get;

	ofMesh face, line;
	ofTrueTypeFont font;
};