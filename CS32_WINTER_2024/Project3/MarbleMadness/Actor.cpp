#include "Actor.h"
#include "StudentWorld.h"


void Pea::doSomething(){
	if(!isAlive())
		return;
	hitStatusCheck();
	if(!isAlive()){
		return;
	}
	getWorld()->moveActor(this, getDirection());
	hitStatusCheck();
}
bool Exit::canBeHit(){
	if(getWalkable()){
		return false;
	}
	return true;
}
void Factory::doSomething(){
	if(getWorld()->canFactorySpawn(this)){
		if(randInt(1, 50) == 1){
			getWorld()->generate_thief(getX(), getY(), isMean());
		}
	}
}
bool Exit::getHit(){
	return canBeHit();
}
void Avatar::doSomething(){
	if(!isAlive())
		return;
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
			case KEY_PRESS_ESCAPE:
				setHealth(0);
			case KEY_PRESS_LEFT:
			 //... move avatar to the left ...;
				getWorld()->moveActor(this, left, true);
			 	break;
			case KEY_PRESS_RIGHT:
				getWorld()->moveActor(this, right, true);
			 //... move avatar to the right ...;
			 	break;
			case KEY_PRESS_UP:
			 	getWorld()->moveActor(this, up, true);
			 	break;
			case KEY_PRESS_DOWN:
				getWorld()->moveActor(this, down, true);
				break;
			case KEY_PRESS_SPACE:
				shoot();
				break;
			case KEY_PRESS_TAB:
				getWorld()->makeItWin();
				break;
		}
	}
}
bool Actor::canBeHit(){
	return true;
}
bool Actor::getHit(){
	m_health -= 2;
	return canBeHit();
}
void Shooter::shoot(){
	if(m_peas > 0)
		m_peas--;
	else if(m_peas == 0)
		return;
	shoot_noise();
	getWorld()->generate_pea(getX(), getY(), getDirection());
}

void Actor::doSomething(){
	return;
}
void Pickup::doSomething(){
	if(!isAlive())
		return;
	getWorld()->avatar_pickup(this);
}
bool Actor::canBeNeighbor(){
	return m_walkable;
}
void Robot::shoot_noise(){
	std::cout << "playing shoot noise for robot" << std::endl;
	getWorld()->playSound(SOUND_ENEMY_FIRE);
}
void Avatar::shoot_noise(){
	std::cout << "playing sound even though my computer won't" << std::endl;
	getWorld()->playSound(SOUND_PLAYER_FIRE);
}

void Pea::hitStatusCheck(){
	if(!isAlive())
		return;
	if(getWorld()->pea_status_check(this)){
		setHealth(0);
	}
}
bool Exit::canBeNeighbor(){
	return true;
}
bool Pit::fillMe(){
	setHealth(0);
	return true;
}


void Pickup::giveScoreAndDie(int bonus){
	getWorld()->addScore(bonus);
	setHealth(0);
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

bool Marble::pushMe(int dir){
	return getWorld()->pushMarble(this, dir);
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Ammo::giveBonus(Avatar* picker){
	giveScoreAndDie(100);
	picker->reload();
}

bool Avatar::getHit(){
	bool returnValue = Actor::getHit();
	if(isAlive()){
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
		return returnValue;
	}
	getWorld()->playSound(SOUND_PLAYER_DIE);
	return returnValue;
}

bool Robot::getHit(int score){
	bool returnValue = Actor::getHit();
	if(isAlive()){
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
		return returnValue;
	}
	getWorld()->playSound(SOUND_ROBOT_DIE);
	getWorld()->addScore(score);
	return returnValue;
}
Robot::Robot(StudentWorld* world, int imageId, double startX, double startY, int startHealth, int dir):
	Shooter(world, imageId, startX, startY, startHealth, dir, -1), m_currentTick(0)
{
	m_tickRate = (28 - getWorld()->getLevel()) / 4;

	if(m_tickRate < 3)
		m_tickRate = 3;
}

RageBot::RageBot(StudentWorld* world, double startX, double startY, int dir):
	Robot(world, IID_RAGEBOT, startX, startY, 10, dir)
{}


ThiefBot::ThiefBot(StudentWorld* world, double startX, double startY, bool mean, int bonus):
	Robot(world, mean ? IID_MEAN_THIEFBOT : IID_THIEFBOT, startX, startY, 5, right), m_mean(mean), 
	m_Pick(nullptr), m_currentDistance(0), m_bonus(bonus)
{
	m_distanceBeforeTurning = randInt(1,6);
}
void Exit::giveBonus(Avatar* picker){
	if(m_revealed == false){
		return;
	}
	getWorld()->playSound(SOUND_FINISHED_LEVEL);
	getWorld()->addScore(2000+getWorld()->getBonus());
	getWorld()->makeItWin();
}

bool Exit::revealMe(){
	m_revealed = true;
	setVisible(true);
	return true;
}

bool Robot::myTick(){
	if(m_currentTick != m_tickRate){
		m_currentTick++;
		return false;
	}
	m_currentTick = 0;
	return true;
}
bool Robot::didBasicTask(bool toShoot){
	if(!isAlive())
		return true;
	if(!myTick()){
		return true;
	}
	if(toShoot && getWorld()->avatarInSight(this)){
		shoot();
		return true;
	}
	return false;
}

bool ThiefBot::getHit(){
	bool returnValue = Robot::getHit(m_bonus);
	if(!isAlive()){
		if(m_Pick != nullptr){

			getWorld()->generate_actor(getX(), getY(), m_Pick);
			m_Pick = nullptr;
		}
	}
	return returnValue;
}

ThiefBot::~ThiefBot(){
	delete m_Pick;
}
void ThiefBot::doSomething(){
	if(didBasicTask(m_mean)){
		return;
	}
	if(m_Pick == nullptr){
		m_Pick = getWorld()->thiefSomething(this);
		if(m_Pick != nullptr){
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			return;
		}
	}
	if(m_currentDistance < m_distanceBeforeTurning){
		if(getWorld()->moveActor(this, getDirection(), false)){
			m_currentDistance++;
			return;
		}
	}
	m_distanceBeforeTurning = randInt(1,6);
	m_currentDistance = 0;
	int randomDirection = randInt(0,3);
	int randomDirectionCopy = randomDirection;
	while(true){
		switch(randomDirection){
		case 1:
			setDirection(left);
			break;
		case 2:
			setDirection(right);
			break;
		case 3:
			setDirection(up);
			break;
		case 0:
			setDirection(down);
			break;
		}
		if(getWorld()->moveActor(this, getDirection(), false)){
			m_currentDistance++;
			return;
		}
		randomDirection++;
		randomDirection = randomDirection % 4;
		if(randomDirection == randomDirectionCopy){
			return;
		}
	}




}

void RageBot::doSomething(){
	if(didBasicTask(true)){
		return;
	}
	if(getWorld()->moveActor(this, getDirection(), false)){
		return;
	}
	switch(getDirection())
	{
	case left:
		setDirection(right);
		break;
	case right:
		setDirection(left);
		break;
	case up:
		setDirection(down);
		break;
	case down:
		setDirection(up);
		break;
	}
}


void Crystal::giveBonus(Avatar* picker){
	giveScoreAndDie(50);
	getWorld()->killCrystal();
}

void Life::giveBonus(Avatar* picker){
	giveScoreAndDie(1000);
	getWorld()->incLives();
}
void Health::giveBonus(Avatar* picker){
	giveScoreAndDie(500);
	picker->heal();
}