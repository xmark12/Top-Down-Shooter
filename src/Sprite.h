#pragma once

#include "Shape.h"



class TriangleShape : public Shape {
public:
	TriangleShape() {
		// default data
		set(glm::vec3(20, 20, 0), glm::vec3(0, -40, 0), glm::vec3(-20, 20, 0));
	}
	virtual void draw();
	virtual bool inside(const glm::vec3 p);

	void set(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
		verts.clear();
		verts.push_back(v1);
		verts.push_back(v2);
		verts.push_back(v3);
	}
};

class Sprite : public TriangleShape {
public:
	float width;
	float height;
	float playerWidth;
	float playerHeight;
	float enemyHeight;
	float speed;
	float rotSpeed;
	float birthtime = 0; // elapsed time in ms
	float lifespan = -1;  //  time in ms
	bool bShowImage = false;
	glm::vec3 velo = glm::vec3(0, 0, 0);
	ofImage picture;

	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	glm::vec3 position = glm::vec3(0, 0, 0);
	float mass = 1.0;
	float damping = .99;
	float force = 0;
	float angularForce = 0;
	float angularVelocity = 0.0;
	float angularAccleration = 0.0;
	bool bThrust = false; //when true, play a sound

	float framerate = ofGetFrameRate();
	float dt = 1.0 / framerate;
	//float damping; //set it as a slider like the rest

	ofVec3f colVal = (verts[0] - verts[1]) / 2;
	float radiusCol = colVal.x;

	Sprite() {
		playerWidth = 50;
		playerHeight = 50;
	}

	glm::vec3 translate(glm::vec3 trans) {
		return glm::vec3(getMatrix() * glm::vec4(trans, 1));
	}

	void setScale(float scale) {
		sizeChanger = glm::vec3(scale, scale, scale);
	}

	void setPlayerSize(float w, float h) {
		playerWidth = w;
		playerHeight = h;
	}

	void setSpeed(float spd) {
		speed = spd;
	}

	void setRotation(float rotSpd) {
		rotSpeed = rotSpd;
	}

	float age() {
		return (ofGetElapsedTimeMillis() - birthtime);
	}

	void imageStats(ofImage image) {
		picture = image;
		bShowImage = true;
		width = image.getWidth();
		height = image.getHeight();
		//cout << "image is loaded" << endl;
	}

	void draw() {
		if (bShowImage) {
			ofPushMatrix();
			//ofSetColor(ofColor::white);
			ofMultMatrix(getMatrix());
			picture.draw(-picture.getWidth() / 2, -picture.getHeight() / 2.0);
			ofPopMatrix();
		}
		else
		{
			TriangleShape::draw();
		}
	}

	glm::vec3 heading() {
		return (glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1)) * glm::vec4(0, 1, 0, 1));
	}

	void drawHeading() {
		glm::vec3 start = translate(glm::vec3(0, 0, 0));
		glm::vec3 end = glm::vec3(0, 0, 0) - glm::vec3(0, 1000, 0);
		end = getMatrix() * glm::vec4(end, 1.0);
		ofDrawLine(start, end);
	}

	void integrate() {

		//linear motion 
		pos += (velocity * dt);
		glm::vec3 accel = acceleration;
		accel += (force * 1.0 / mass); //set force to 0 upon letting go of button, force is a slider
		velocity += accel * dt;
		velocity *= damping;

		//angular rotation
		rot += (angularVelocity * dt);
		float a = 0;
		a += (angularForce * 1.0 / mass);
		angularVelocity += a * dt;
		angularVelocity *= damping;

	}
};

