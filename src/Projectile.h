#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "Sprite.h"

//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Projectile : public Shape {
public:
	Projectile();
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
	virtual void moveSprite(Sprite*);
	float getAgentPos();
	virtual void spawnSprite();

	void setChildSize(float w, float h) { childWidth = w; childHeight = h; }
	void setPlayerSprite(Sprite* p) { playerSprite = p; }
	Sprite getPlayerSprite() { return *playerSprite; }

	SpriteList* sys;
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
	bool projectileLaunched = false;
	Sprite *playerSprite;
};

