#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->sphere_list.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	int span = 30;
	float threshold = 0.48;
	float frame_param = ofGetFrameNum() * 0.003;
	float coordinate_param = 0.0065;
	int z = 300 + span * 0.5;

	float angle;
	glm::highp_mat4 rotation;
	for (int i = 0; i <= 6; i++) {

		for (int x = -300; x <= 300; x += span) {

			for (int y = -300; y <= 300; y += span) {

				if (i <= 4) {

					angle = PI * 0.5 * i;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0));
				}

				if (i == 5) {

					angle = PI * 0.5;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
				}

				if (i == 6) {

					angle = PI * 0.5 * 3;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
				}


				glm::vec3 noise_location = glm::vec4(x, y, z , 0) * rotation;
				auto noise_value = ofNoise(glm::vec4(noise_location * coordinate_param, frame_param));
				if (noise_value < threshold) { continue; }

				glm::vec3 loc_1 = glm::vec4(x - span, y, z, 0) * rotation;
				auto noise_1 = ofNoise(glm::vec4(loc_1 * coordinate_param, frame_param));
				glm::vec3 loc_2 = glm::vec4(x, y + span , z, 0) * rotation;
				auto noise_2 = ofNoise(glm::vec4(loc_2 * coordinate_param, frame_param));
				glm::vec3 loc_3 = glm::vec4(x + span, y, z, 0) * rotation;
				auto noise_3 = ofNoise(glm::vec4(loc_3 * coordinate_param, frame_param));
				glm::vec3 loc4 = glm::vec4(x, y - span, z, 0) * rotation;
				auto noise_4 = ofNoise(glm::vec4(loc4 * coordinate_param, frame_param));

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec4(x - span * 0.5, y - span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x + span * 0.5, y - span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x + span * 0.5, y + span * 0.5, z, 0) * rotation);
				vertices.push_back(glm::vec4(x - span * 0.5, y + span * 0.5, z, 0) * rotation);

				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 2);
				this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);

				if (noise_1 < threshold || x == -300) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_2 < threshold || y == 300) {

					this->frame.addVertex(vertices[2]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_3 < threshold || x == 300) {

					this->frame.addVertex(vertices[1]);
					this->frame.addVertex(vertices[2]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_4 < threshold || y == -300) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[1]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);

				}
			}
		}
	}

	this->sphere_list.clear();

	vector<ofColor> base_color_list;
	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		base_color_list.push_back(color);
	}

	int color_index = 0;
	for (int i = 0; i < 150; i++) {

		auto x = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0005), 0, 1, -250, 250);
		auto y = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0005), 0, 1, -250, 250);
		auto z = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0005), 0, 1, -250, 250);

		this->sphere_list.addVertex(glm::vec3(x, y, z));

		this->sphere_list.addColor(base_color_list[color_index]);
		color_index = (color_index + 1) % base_color_list.size();
	}

	for (int i = 0; i < this->sphere_list.getNumVertices(); i++) {

		for (int k = i + 1; k < this->sphere_list.getNumVertices(); k++) {

			if (glm::distance(this->sphere_list.getVertex(i), this->sphere_list.getVertex(k)) < 50) {

				this->sphere_list.addIndex(i);
				this->sphere_list.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);
	ofRotateX(ofGetFrameNum() * 0.75);

	ofSetColor(39);
	this->frame.draw();

	ofSetColor(239);
	this->face.draw();

	this->sphere_list.drawWireframe();
	for (int i = 0; i < this->sphere_list.getNumVertices(); i++) {

		ofSetColor(this->sphere_list.getColor(i));
		ofDrawSphere(this->sphere_list.getVertex(i), 8);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}