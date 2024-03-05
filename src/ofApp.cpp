#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);
	ofSetFrameRate(60);

	// Record when the game starts - game will run for 10 sec
	//
	gameStartTime = 0;

	//create background
	//
	if (!background.load("images/base.png")) {
		cout << "Can't load image" << endl;
		ofExit();
	}
	
	// create an image for sprites being spawned by emitter
	//
	if (defaultImage.load("images/enemy.png")) {
		imageLoaded = true;
	}
	else {
		cout << "Can't open image file" << endl;
		ofExit();
	}

	// Load example image
	//
	if (!playerImage.load("images/girl.png")) {
		cout << "Can't load image" << endl;
		ofExit();
	}

	//sprite = Player(glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0, -50, 0));
	player.setScale(1.0);
	player.pos = glm::vec3(ofGetWindowWidth() / 2.0 - 100, ofGetWindowHeight() / 2.0 - 100, 0);
	ofSetColor(ofColor::white);
	player.imageStats(playerImage);
	player.rot = 0;

	// create an array of emitters and set their position;
	//
	
	turret = new Emitter();
	gun = new Projectile();

	//turret = new AgentEmitter();

	turret->pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
	turret->drawable = true; 
	turret->setChildImage(defaultImage);
	turret->start();

	gun->pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
	gun->drawable = true;
	//gun->setChildImage(defaultImage);
	gun->start();
	
	gui.setup();
	gui.add(spdSlider.setup("Ship Speed", 10, 0.0, 100));
	gui.add(rotSlider.setup("Rotation Speed", 3.5, 0.0, 10));
	gui.add(scaSlider.setup("Scale", 0.5, 0.5, 5));
	gui.add(imgButton.setup("Player Sprite", true));
	gui.add(agentButton.setup("Agent Sprite", true));
	gui.add(headToggle.setup("Heading", false));
	gui.add(enrSlider.setup("Max Energy", 3, 1, 10));
	gui.add(rate.setup("Enemy Rate", 1, 0.5, 10));
	gui.add(velocityInc.setup("Enemy Velocity", 1.0, 1.0, 5));
	gui.add(life.setup("Enemy Life", 5, 0, 10));
	gui.add(levelSlider.setup("Level", 1, 1, 4));
	bHide = false;

	playerMove.load("sounds/rocketPlayer.wav"); //can be mp3 or wav
	playerMove.setLoop(true); //sets loop, set to false for stuff like gun shots
	playerMove.setVolume(0.5f); //sets volume to 50% of maximum, 0.0 is silent and 1.0 is full

	playerShoot.load("sounds/projectileShoot.wav"); //can be mp3 or wav
	playerShoot.setLoop(false); //sets loop, set to false for stuff like gun shots
	playerShoot.setVolume(0.3f); //sets volume to 50% of maximum, 0.0 is silent and 1.0 is full

	hit.load("sounds/playerHit.wav"); //can be mp3 or wav
	hit.setLoop(false); //sets loop, set to false for stuff like gun shots
	hit.setVolume(1.0f); //sets volume to 50% of maximum, 0.0 is silent and 1.0 is full

	projectileHit.load("sounds/enemyHit.wav"); //can be mp3 or wav
	projectileHit.setLoop(false); //sets loop, set to false for stuff like gun shots
	projectileHit.setVolume(0.8f); //sets volume to 50% of maximum, 0.0 is silent and 1.0 is full

	playerDead.load("sounds/playerKilled.wav"); //can be mp3 or wav
	playerDead.setLoop(false); //sets loop, set to false for stuff like gun shots
	playerDead.setVolume(1.0f); //sets volume to 50% of maximum, 0.0 is silent and 1.0 is full

	menuMusic.load("sounds/mainMenu.mp3"); //can be mp3 or wav
	menuMusic.setLoop(true); //sets loop, set to false for stuff like gun shots
	menuMusic.setVolume(0.3f); //sets volume to 50% of maximum, 0.0 is silent and 1.0 is full

	battleMusic.load("sounds/battleMusic.mp3"); //can be mp3 or wav
	battleMusic.setLoop(true); //sets loop, set to false for stuff like gun shots
	battleMusic.setVolume(0.3f); //sets volume to 50% of maximum, 0.0 is silent and 1.0 is full

	// set up the emitter forces
	//
	turbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
	gravityForce = new GravityForce(ofVec3f(0, -10, 0));
	radialForce = new ImpulseRadialForce(1000.0);


	emitter.sys->addForce(turbForce);
	emitter.sys->addForce(gravityForce);
	emitter.sys->addForce(radialForce);

	emitter2.sys->addForce(turbForce);
	emitter2.sys->addForce(gravityForce);
	emitter2.sys->addForce(radialForce);

	emitter.setVelocity(ofVec3f(0, 0, 0));
	emitter.setOneShot(true);
	emitter.setEmitterType(RadialEmitter);
	emitter.setGroupSize(5000);

	emitter2.setVelocity(ofVec3f(0, 0, 0));
	emitter2.setOneShot(true);
	emitter2.setEmitterType(RadialEmitter);
	emitter2.setGroupSize(5000);

}

//--------------------------------------------------------------
void ofApp::update() {
	int totalVelocityIncrease = velocityIncrease + velocityInc;
	
	turret->setRate(rate+rateIncrease);
	turret->setLifespan((life+lifeIncrease) * 1000);    // convert to milliseconds 
	turret->setVelocity(ofVec3f(velocity->x, velocity->y, velocity->z));
	turret->setVelocityValue(&totalVelocityIncrease);
	turret->update();

	//gun->setRate(1);
	gun->setLifespan(5 * 1000);    // convert to milliseconds 
	gun->setVelocity(ofVec3f(0, -500, 0));
	gun->update();

	player.setScale(scaSlider);
	player.setSpeed(spdSlider);
	player.setRotation(rotSlider);

	turret->setPlayerPos(player.pos);
	turret->sys->setPlayerSprite(&player);

	int maxEnergy = enrSlider;
	nEnergy = enrSlider - energyDecrease + turret->sys->regenEnergy;
	level = levelSlider + levelChange;
	turret->sys->setCurrentEnergy(&nEnergy);
	turret->sys->setEnergySlider(&maxEnergy);
	turret->sys->setEnergyDecrease(&energyDecrease);
	turret->sys->setSound(&projectileHit);

	player.bShowImage = imgButton;

	gun->pos = player.pos;
	gun->setPlayerSprite(&player);

	for (int i = 0; i < turret->sys->sprites.size(); i++) {
		turret->sys->sprites[i].bShowImage = agentButton;
	}

	radialForce->set(1000, 0);

	emitter.setLifespan(2);
	emitter.setVelocity(ofVec3f(100, 100, 100));
	emitter.setRate(rate);
	emitter.setParticleRadius(0.3);
	//turret->sys->setRingEmitter(&emitter);
	emitter.update();

	emitter2.setLifespan(2);
	emitter2.setVelocity(ofVec3f(100, 100, 20));
	emitter2.setRate(rate);
	emitter2.setParticleRadius(0.3);
	//turret->sys->setCircleEmitter(&emitter2);
	emitter2.update();

	if (keymap['W'] || keymap['w'] || keymap[OF_KEY_UP]) {
		player.velo = player.speed * player.heading();
		//cout << "moving" << endl;
		if ((player.pos.y <= ofGetWindowHeight()) && (player.pos.y >= 0) && (player.pos.x <= ofGetWindowWidth()) && (player.pos.x >= 0)) {
			player.pos -= player.velo;
			//player.velocity -= player.speed * player.heading();
		}
		else if (player.pos.y > ofGetWindowHeight()) {
			player.pos.y -= 1;
		}
		else if (player.pos.y < 0) {
			player.pos.y += 1;
		}
		else if (player.pos.x > ofGetWindowHeight()) {
			player.pos.x -= 1;
		}
		else if (player.pos.x < 0) {
			player.pos.x += 1;
		}
	}
	if (keymap['A'] || keymap['a'] || keymap[OF_KEY_LEFT]) {
		player.rot -= player.rotSpeed;
		//player.angularVelocity -= player.rotSpeed;
		//cout << "moving" << endl;
	}
	if (keymap['S'] || keymap['s'] || keymap[OF_KEY_DOWN]) {
		player.velo = player.speed * player.heading();
		//cout << "moving" << endl;
		if ((player.pos.y <= ofGetWindowHeight()) && (player.pos.y >= 0) && (player.pos.x <= ofGetWindowWidth()) && (player.pos.x >= 0)) {
			player.pos += player.velo;
			//player.velocity += player.speed * player.heading();
		}
		else if (player.pos.y > ofGetWindowHeight()) {
			player.pos.y -= 1;
		}
		else if (player.pos.y < 0) {
			player.pos.y += 1;
		}
		else if (player.pos.x > ofGetWindowHeight()) {
			player.pos.x -= 1;
		}
		else if (player.pos.x < 0) {
			player.pos.x += 1;
		}
	}
	if (keymap['D'] || keymap['d'] || keymap[OF_KEY_RIGHT]) {
		player.rot += player.rotSpeed;
		//player.angularVelocity += player.rotSpeed;
		//cout << "moving" << endl;
	}

	if (keymap[' '] && gameOver == true) {
		gameOver = false;
		gameBegin = false;
	}

	if (!gameBegin) {
		ofResetElapsedTimeCounter();
		level = 1;
		finalTime = 0;
		nEnergy = 0;
		energyDecrease = 0;
		rateIncrease = 0;
		lifeIncrease = 0;
		velocityIncrease = 1;
		levelChange = 0;
		score = 0;
		scoreDecrease = 0;
		turret->sys->scoreIncrease = 0;
		turret->sys->regenEnergy = 0;
		player.velocity = glm::vec3(0, 0, 0);

		turret->lastSpawned = 0;
		gun->lastSpawned = 0;
		gameOver = false;
		
		if(!menuMusic.isPlaying())
			menuMusic.play();
		battleMusic.stop();
	}
	else if (gameBegin) {
		menuMusic.stop();
		if(!battleMusic.isPlaying())
			battleMusic.play();
		//check thrusters for sound
		if (player.bThrust && !playerMove.isPlaying()) {
			playerMove.play();
		}
		else if (!player.bThrust && playerMove.isPlaying()) {
			playerMove.stop();
		}
		player.integrate();
		for (int i = 0; i < turret->sys->sprites.size(); i++) {
			if (abs(glm::distance(player.pos, turret->sys->sprites.at(i).pos)) < (turret->sys->sprites.at(i).radiusCol + player.radiusCol)) {
				turret->sys->sprites.at(i).lifespan = 0;
				energyDecrease += 1;
				scoreDecrease += 100;
				if (nEnergy <= 1) {
					playerDead.play();
					battleMusic.stop();
					//cout << "dead" << endl;
					for (int i = 0; i < 3000; i++) {
						cout << i << endl;
					}
					projectileHit.play();
				}
				else if (nEnergy > 1) {
					hit.play();
					emitter.setPosition(player.pos);
					emitter.start();
					//cout << "hit" << endl;
					for (int i = 0; i < 500; i++) {
						cout << i << endl;
					}
				}
				
			}
			if (gun->projectileLaunched) {
				float collisionDist = gun->childHeight / 2 + turret->childHeight / 2;
				for (int j = 0; j < gun->sys->sprites.size(); j++) {
					turret->sys->removeNear(gun->sys->sprites[j].pos, collisionDist);

				}
				gun->projectileLaunched = false;
			}
		}
		
		if (finalTime >= 23) {
			levelChange = 1;
		}

		if (finalTime >= 48) {
			levelChange = 2;
		}

		if (finalTime >= 90) {
			levelChange = 3;
		}
		
		if (level == 1) {
			rateIncrease = 0;
			lifeIncrease = 0;
			velocityIncrease = 1;
		}
		
		if (level == 2) {
			rateIncrease = 2;
			lifeIncrease = 2;
			velocityIncrease = 1.5;
		}

		if (level == 3) {
			rateIncrease = 4;
			lifeIncrease = 4;
			velocityIncrease = 2;
		}
		
		if (level == 4) {
			rateIncrease = 6;
			lifeIncrease = 6;
			velocityIncrease = 2.5;
		}

		score = turret->sys->getScoreIncrease() - scoreDecrease;

		if (nEnergy <= 0) {
			gameOver = true;
		}
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(ofColor::white);
	background.draw(0, 0);
	if (bHide) gui.draw();
	string startGame;
	startGame += "Press Spacebar to Start Game";
	string endGame;
	endGame += "Game Over. Time Lasted: " + std::to_string(finalTime) + " seconds. Score: " + std::to_string(score) 
			+ ". Press Spacebar to play again.";
	if (!gameBegin) {
		ofDrawBitmapString(startGame, ofPoint(ofGetWindowWidth() / 2.3, ofGetWindowHeight() / 2.0));
		turret->sys->sprites.clear();
		// draw particle emitter here..
		//
		emitter.draw();
		emitter2.draw();
	}
	else if (gameOver) {
		ofDrawBitmapString(endGame, ofPoint(ofGetWindowWidth() / 2.8, ofGetWindowHeight() / 2.0));
		turret->sys->sprites.clear();
		gun->sys->sprites.clear();
		battleMusic.stop();
		// draw particle emitter here..
		//
		emitter.draw();
		emitter2.draw();
	}
	else {
		float time = ofGetElapsedTimeMillis();
		finalTime = (time - gameStartTime) / 1000;
		// draw particle emitter here..
		//
		emitter.draw();
		emitter2.draw();
		player.draw();
		if (headToggle) {
			player.drawHeading();
		}
		turret->draw();
		gun->draw();
	}
	ofSetColor(ofColor::white);
	string str;
	str += "Frame Rate: " + std::to_string(ofGetFrameRate());
	ofDrawBitmapString(str, ofGetWindowWidth() / 1.17, 120);
	string scoreTutorial2;
	scoreTutorial2 += "Losing Energy decreases Score.";
	ofDrawBitmapString(scoreTutorial2, ofPoint(ofGetWindowWidth() / 1.17, 100));
	string scoreTutorial1;
	scoreTutorial1 += "Shoot enemies to increase Score.";
	ofDrawBitmapString(scoreTutorial1, ofPoint(ofGetWindowWidth() / 1.17, 80));
	string shootText;
	shootText += "Press P to shoot.";
	ofDrawBitmapString(shootText, ofPoint(ofGetWindowWidth() / 1.17, 60));
	string restartText;
	restartText += "Press R to restart the game.";
	ofDrawBitmapString(restartText, ofPoint(ofGetWindowWidth() / 1.17, 40));
	string fullText;
	fullText += "Press F to go into fullscreen.";
	ofDrawBitmapString(fullText, ofPoint(ofGetWindowWidth() / 1.17, 20));
	string levelText;
	levelText += "Level: " + std::to_string(level);
	ofDrawBitmapString(levelText, ofPoint(ofGetWindowWidth() / 2.2, 80));
	string scoreText;
	scoreText += "Score: " + std::to_string(score);
	ofDrawBitmapString(scoreText, ofPoint(ofGetWindowWidth() / 2.2, 60));
	string currentTime;
	currentTime += "Time: " + std::to_string(finalTime);
	ofDrawBitmapString(currentTime, ofPoint(ofGetWindowWidth() / 2.2, 40));
	string currentEnergy;
	currentEnergy += "Energy Remaining: " + std::to_string(nEnergy) + "/" + std::to_string(enrSlider);
	ofDrawBitmapString(currentEnergy, ofPoint(ofGetWindowWidth() / 2.2, 20));
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {
	keymap[key] = true;
	
	switch (key) {
	case 'W':
	case 'w':
	case 'S':
	case 's':
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		player.bThrust = true;
		break;
	case 'C':
	case 'c':
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	case 'R':
	case 'r':
		gameBegin = false;
		break;
	case 'u':
		break;
	case 'P':
	case 'p':
		if (gameBegin && !gameOver) {
			gun->spawnSprite();
			playerShoot.play();
		}
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case ' ':
		gameBegin = true;
		break;
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	keymap[key] = false;
	
	switch (key) {
	case 'W':
	case 'w':
	case 'S':
	case 's':
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		player.bThrust = false;
		break;
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

