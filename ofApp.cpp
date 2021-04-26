#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(2);
	ofEnableDepthTest();
	
	this->sound.load("sound/coe-font-studio_20210426.wav"); // Voiced by https://CoeFont.studio
	this->sound.setLoop(true);
	this->sound.play();

	this->n_bands_to_get = 128;
	for (int j = 0; j < this->n_bands_to_get; j++) {

		this->fft_value.push_back(0.f);
		this->fft_smoothed.push_back(0.f);
		this->deg_list.push_back(0);
	}

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 30, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSoundUpdate();

	float* val = ofSoundGetSpectrum(this->n_bands_to_get);
	for (int i = 0; i < this->n_bands_to_get; i++) {

		this->fft_value[i] = val[i];
		this->fft_smoothed[i] *= 0.98f;
		if (this->fft_smoothed[i] < val[i]) {

			this->fft_smoothed[i] = val[i];
		}

		this->deg_list[i] = ofMap(this->fft_smoothed[i], 0, 1, 0, 1000);
		if (this->deg_list[i] > 180) { this->deg_list[i] = 180; }
	}

	this->face.clear();
	this->line.clear();

	for (int i = 0; i < 7; i++) {

		auto radius = i * 50 + 50;
		this->setRingToMesh(this->face, this->line, glm::vec3(), radius, 25, 30, 0, this->deg_list[i]);
		this->setRingToMesh(this->face, this->line, glm::vec3(), radius, 25, 30, 180, 180 + this->deg_list[i]);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(15 * sin(ofGetFrameNum() * 0.015));

	ofSetColor(39);
	this->face.draw();

	auto word = "Voiced by https ://CoeFont.studio";
	this->font.drawString(word, this->font.stringWidth(word) * -0.5, -420);

	
	ofSetColor(239);
	this->line.drawWireframe();	

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& line_target, glm::vec3 location, float radius, float width, float height, int deg_start, int deg_end) {

	if (deg_start > deg_end) {

		auto tmp = deg_start - 1;
		deg_start = deg_end;
		deg_end = tmp;
	}

	int index = face_target.getNumVertices();
	int deg_span = 1;
	for (int deg = deg_start; deg <= deg_end; deg += deg_span) {

		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			vertex += location;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2);

		auto frame_index = line_target.getNumVertices();

		line_target.addVertices(vertices);

		line_target.addIndex(frame_index + 0); line_target.addIndex(frame_index + 1);
		line_target.addIndex(frame_index + 2); line_target.addIndex(frame_index + 3);
		line_target.addIndex(frame_index + 4); line_target.addIndex(frame_index + 5);
		line_target.addIndex(frame_index + 6); line_target.addIndex(frame_index + 7);
	}

	face_target.addIndex(index + 0); face_target.addIndex(index + 3); face_target.addIndex(index + 7);
	face_target.addIndex(index + 0); face_target.addIndex(index + 7); face_target.addIndex(index + 4);

	line_target.addIndex(index + 0); line_target.addIndex(index + 3);
	line_target.addIndex(index + 0); line_target.addIndex(index + 4);
	line_target.addIndex(index + 7); line_target.addIndex(index + 3);
	line_target.addIndex(index + 7); line_target.addIndex(index + 4);

	index = line_target.getNumVertices() - 8;

	face_target.addIndex(index + 1); face_target.addIndex(index + 2); face_target.addIndex(index + 6);
	face_target.addIndex(index + 1); face_target.addIndex(index + 6); face_target.addIndex(index + 5);

	line_target.addIndex(index + 1); line_target.addIndex(index + 2);
	line_target.addIndex(index + 1); line_target.addIndex(index + 5);
	line_target.addIndex(index + 6); line_target.addIndex(index + 2);
	line_target.addIndex(index + 6); line_target.addIndex(index + 5);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}