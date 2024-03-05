#include "ofApp.h"
#include "Projectile.h"
//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Projectile" class to emit Sprites
// and set them in motion. The concept of an "Projectile" is taken from particle
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


Projectile::Projectile() {
	sys = new SpriteList();
	init();
}


void Projectile::init() {
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
void Projectile::draw() {
	if (drawable) {
		ofSetColor(134, 241, 255);
		//ofDrawRectangle(-width / 2 + pos.x, -height / 2 + pos.y, width, height);
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Projectile::update() {
	if (!started) return;
	/*float time = ofGetElapsedTimeMillis();

	if ((time - lastSpawned) > (1000.0 / rate)) {

		// call virtual to spawn a new sprite
		//
		spawnSprite();
		lastSpawned = time;
	}*/
	



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

void Projectile::setPlayerPos(glm::vec3 position) {
	direction = position;
}

ofVec3f Projectile::getPlayerPos() {
	return direction;
}

// virtual function to move sprite (can be overloaded)
//
void Projectile::moveSprite(Sprite* sprite) {
	sprite->pos += sprite->velocity / ofGetFrameRate();
}

// virtual function to spawn sprite (can be overloaded)
//
void Projectile::spawnSprite() {
	Sprite sprite;
	ofApp getPlayer;
	sprite.velocity = -1000 * getPlayerSprite().heading();
	sprite.lifespan = lifespan;
	sprite.pos = pos;
	sprite.rot = getPlayerSprite().rot;
	sprite.birthtime = ofGetElapsedTimeMillis();
	sprite.enemyHeight = childHeight / 2;
	sprite.colVal = (sprite.verts[0] - sprite.verts[1]) / 2;
	sprite.radiusCol = sprite.colVal.x;
	projectileLaunched = true;
	sys->add(sprite);
}

// Start/Stop the emitter.
//
void Projectile::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Projectile::stop() {
	started = false;
}


void Projectile::setLifespan(float life) {
	lifespan = life;
}

void Projectile::setVelocity(const glm::vec3 v) {
	velocity = v;
}

void Projectile::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Projectile::setImage(ofImage img) {
	image = img;
}

void Projectile::setRate(float r) {
	rate = r;
}
