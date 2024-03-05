#include "ofApp.h"
//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" class to emit Sprites
// and set them in motion. The concept of an "Emitter" is taken from particle
// systems (which we will cover next week).
//
// The Sprite class has also been upgraded to include lifespan, velocity and age
// members.   The emitter can control rate of emission and the current velocity
// of the particles. In this example, there is no acceleration or physics, the
// sprites just move simple frame-based animation.
//
// The code shows a way to attach images to the sprites and optional the
// emitter (which is a point source) can also have an image.  If there are
// no images attached, a placeholder rectangle is drawn.
// Emitters  can be placed anywhere in the window. In this example, you can drag
// it around with the mouse.
//
// OF has an add-in called ofxGUI which is a very simple UI that is useful for
// creating sliders, buttons and fields. It is not recommended for commercial 
// game development, but it is useful for testing.  The "h" key will hide the GUI
// 
// If you want to run this example, you need to use the ofxGUI add-in in your
// setup.
//
//
//  Kevin M. Smith - CS 134 SJSU




//  Add a Sprite to the Sprite System
//
void SpriteList::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteList::remove(int i) {
	sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteList::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].pos += sprites[i].velo / ofGetFrameRate();
	}
}

//  Render all the sprites
//
void SpriteList::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}

// remove all sprites within a given dist of point, return number removed
//
int SpriteList::removeNear(ofVec3f point, float dist) {
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;
	int count = 0;

	while (s != sprites.end()) {
		ofVec3f v = s->pos - point;
		if (v.length() < dist) {
			glm::vec3 enemyPos = s->pos;
			//getRingEmitter().position = enemyPos;
			//getCircleEmitter().position = enemyPos;
			//getRingEmitter().sys->reset();
			//getRingEmitter().start();
			//getCircleEmitter().sys->reset();
			//getCircleEmitter().start();
			tmp = sprites.erase(s);
			count++;
			s = tmp;
			getSound().play();
			scoreIncrease += 100;
			if (getCurrentEnergy() < getEnergySlider()) {
				regenEnergy += 0;
			}
			else {
				regenEnergy = regenEnergy;
			}
			//for (int i = 0; i < 200; i++) {
			//	cout << i << endl;
			//}
		}
		else s++;
	}
	return count;
}

Emitter::Emitter() {
	sys = new SpriteList();
	init();
}


void Emitter::init() {
	lifespan = 3000;    // default milliseconds
	started = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
	childWidth = 50;
	childHeight = 50;
}



//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			//image.draw(-image.getWidth() / 2.0 + pos.x, -image.getHeight() / 2.0 + pos.y);
		}
		else {
			//ofSetColor(0, 0, 200);
			//ofDrawRectangle(-width / 2 + pos.x, -height / 2 + pos.y, width, height);
		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (!started) return;

	float time = ofGetElapsedTimeMillis();

	if ((time - lastSpawned) > (1000.0 / rate)) {

		// call virtual to spawn a new sprite
		//
		spawnSprite();
		lastSpawned = time;
	}



	// update sprite list
	//
	if (sys->sprites.size() == 0) return;
	vector<Sprite>::iterator s = sys->sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sys->sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sys->sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	
	for (int i = 0; i < sys->sprites.size(); i++) {
		moveSprite(&sys->sprites[i]);
	}
	
}

void Emitter::setPlayerPos(glm::vec3 position) {
	direction = position;
}

ofVec3f Emitter::getPlayerPos() {
	return direction;
}

// virtual function to move sprite (can be overloaded)
//
void Emitter::moveSprite(Sprite *sprite) {
	difference = getPlayerPos() - sprite->pos;
	//sprite->pos += difference / 100;
	sprite->integrate();
	sprite->velocity += getVelocityValue() * (difference/100);

	ofVec3f heading = sprite->heading();
	
	ofVec3f rotatFirst = sprite->pos - getPlayerPos();
	ofVec3f vec1 = rotatFirst.normalize();
	ofVec3f vec2 = heading.normalize();

	float dot = vec1.dot(vec2);
	float theta = acos(dot);

	if (sprite->pos.x < getPlayerPos().x) {
		if (sprite->rot > 90 && sprite->rot < 270) {
			sprite->rot -= theta;
		}
		else {
			sprite->rot += theta;
		}
	}
	if (sprite->pos.x > getPlayerPos().x) {
		if (sprite->rot > 90 && sprite->rot < 270) {
			sprite->rot += theta;
		}
		else {
			sprite->rot -= theta;
		}
	}
	
}

// virtual function to spawn sprite (can be overloaded)
//
void Emitter::spawnSprite() {
	Sprite sprite;
	if (haveChildImage) {
		sprite.imageStats(childImage);
	}
	sprite.velo = velocity;
	sprite.lifespan = lifespan;

	//put random here; also set bounds like u did for the player
	sprite.pos = pos+glm::vec3(ofRandom(-ofGetWindowWidth(), ofGetWindowWidth()), ofRandom(-ofGetWindowHeight(), 
		ofGetWindowHeight()), ofRandom(0, 0));	
	sprite.rot = rot+ofRandom(0,360);
	
	sprite.birthtime = ofGetElapsedTimeMillis();

	sprite.width = childWidth;
	sprite.height = childHeight;

	sprite.colVal = (sprite.verts[0] - sprite.verts[1]) / 2;
	sprite.radiusCol = sprite.colVal.x;

	sprite.enemyHeight = childHeight / 2;

	sys->add(sprite);
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(const glm::vec3 v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
}

void Emitter::setRate(float r) {
	rate = r;
}
