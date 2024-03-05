#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "Sprite.h"

//
//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteList {
public:
	void add(Sprite);
	void remove(int);
	void update();
	void draw();
	int removeNear(ofVec3f point, float dist);
	vector<Sprite> sprites;

	void setPlayerSprite(Sprite* p) { playerSprite = p; }
	Sprite getPlayerSprite() { return *playerSprite; }
	//void setRingEmitter(ParticleEmitter* r) { ringEmitter = r; }
	//ParticleEmitter getRingEmitter() { return *ringEmitter; }
	//void setCircleEmitter(ParticleEmitter* c) { circleEmitter = c; }
	//ParticleEmitter getCircleEmitter() { return *circleEmitter; }
	void setEnergySlider(int* s) { energy = s; }
	int getEnergySlider() { return *energy; }
	void setEnergyDecrease(int* d) { decreaseEnergy = d; }
	int getEnergyDecrease() { return *decreaseEnergy; }
	int getScoreIncrease() { return scoreIncrease; }
	int getRegenEnergy() { return regenEnergy; }
	void setCurrentEnergy(int* c) { currentEnergy = c; }
	int getCurrentEnergy() { return *currentEnergy; }
	void setSound(ofSoundPlayer* e) { hitEnemy = e; }
	ofSoundPlayer getSound() { return *hitEnemy; }

	Sprite* playerSprite;
	//ParticleEmitter* ringEmitter;
	//ParticleEmitter* circleEmitter;
	int* energy;
	int* decreaseEnergy;
	int scoreIncrease = 0;
	int regenEnergy = 0;
	int* currentEnergy;
	ofSoundPlayer* hitEnemy;
};


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public Shape {
public:
	Emitter();
	void init();
	void draw();
	void start();
	void stop();
	void setLifespan(float);
	void setVelocity(const glm::vec3 v);
	void setChildImage(ofImage);
	void setImage(ofImage);
	void setRate(float);
	void update();

	void setPlayerPos(glm::vec3 position);
	ofVec3f getPlayerPos();

	// virtuals - can overloaded
	virtual void moveSprite(Sprite *);
	float getAgentPos();
	virtual void spawnSprite();

	void setChildSize(float w, float h) { childWidth = w; childHeight = h; }
	void setVelocityValue(int *v) { increaseVelocity = v; }
	int getVelocityValue() { return *increaseVelocity; }

	SpriteList *sys;
	float rate;
	glm::vec3 velocity;
	float lifespan;
	bool started;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
	float childWidth, childHeight;
	glm::vec3 direction;
	ofVec3f difference;
	int *increaseVelocity;
};

