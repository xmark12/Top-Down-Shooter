#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Shape.h"
#include "Projectile.h"
#include "Particle.h"
#include "ParticleEmitter.h"

class Agent : public Sprite {

};

class AgentEmitter : public Emitter {
public:
	void moveSprite(Sprite* sprite) {
		Emitter::moveSprite(sprite);
	}
	void spawnSprite() {
		Emitter::spawnSprite();
	}
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
	//	vector<Emitter *> emitters;
	//	int numEmitters;

		Emitter  *turret = NULL;
		Projectile* gun = NULL;
		Sprite player;

		ofImage defaultImage;
		ofImage playerImage;
		ofImage background;
		ofVec3f mouse_last;
		bool imageLoaded;

		bool bHide;

		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxLabel screenSize;

		ofxFloatSlider spdSlider, scaSlider, rotSlider;
		ofxIntSlider enrSlider;
		ofxIntSlider velocityInc;
		ofxToggle imgButton;
		ofxToggle agentButton;
		ofxToggle headToggle;

		ofxIntSlider levelSlider;

		ofxPanel gui;

		// state variables
		//
		bool bInDrag = false;
		bool bInRotate = false;
		bool bCtrlKeyDown = false;
		glm::vec3 mouseLast;

		int level;
		int gameStartTime;
		int finalTime;
		int nEnergy;
		int energyDecrease;

		int rateIncrease;
		int lifeIncrease;
		int velocityIncrease = 1;
		int levelChange;
		int score;
		int scoreDecrease = 0;

		bool gameBegin = false;
		bool gameOver = false;
		bool playerKilled = false;
		bool playerIsMoving = false;
		bool triIsPressed;
		map<int, bool> keymap;

		ofSoundPlayer playerMove;
		ofSoundPlayer playerShoot;
		ofSoundPlayer enemyMove;
		ofSoundPlayer hit;
		ofSoundPlayer projectileHit;
		ofSoundPlayer playerDead;
		ofSoundPlayer menuMusic;
		ofSoundPlayer battleMusic;

		ParticleEmitter emitter;
		ParticleEmitter emitter2;

		// adding forces
		//
		TurbulenceForce* turbForce;
		GravityForce* gravityForce;
		ImpulseRadialForce* radialForce;
};
