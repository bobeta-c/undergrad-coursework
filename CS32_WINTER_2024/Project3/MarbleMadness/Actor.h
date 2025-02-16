#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Pickup;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, int health, bool walkable = false, int dir = none):
		GraphObject(imageID, startX, startY, dir), m_health(health), m_studentWorld(world), m_walkable(walkable)
	{setVisible(true);}
	virtual void doSomething();
	int getHealth() const {return m_health;}
	bool isAlive() const {return getHealth() > 0;}
	StudentWorld* getWorld() const {return m_studentWorld;}
	void setHealth(int newHealth) {m_health = newHealth;}
	bool getWalkable() const {return m_walkable;}
	
	virtual bool revealMe() {return false;}
	virtual bool fillMe() {return false;}
	virtual bool canBeNeighbor();
	virtual bool pushMe(int dir) {return false;}
	virtual bool canBeHit();
	virtual bool getHit();
	virtual bool willSteal() {return false;}
	virtual bool isGoodie() {return false;}


private:
	int m_health;
	StudentWorld* m_studentWorld;
	bool m_walkable; // can objects walk on the same square as me
};

class Immutable: public Actor
{
public:
	Immutable(StudentWorld* world, int imageId, double startX, double startY, bool walkable = false):
		Actor(world, imageId, startX, startY, 999, walkable)
	{}
	virtual bool canBeHit(){
		return true;
	}
	virtual bool getHit(){
		return true;
	}

};

class Shooter: public Actor
{
public:
	Shooter(StudentWorld* world, int imageId, double startX, double startY, int health, int dir, int peas = -1):
		Actor(world, imageId, startX, startY, health, false, dir), m_peas(peas)
	{}

	bool has_peas(){return m_peas != 0;}

	void shoot();
	void reload() {m_peas += 20;}
	int getPeas() {return m_peas;}
	virtual void shoot_noise() = 0;
private:
	int m_peas;
};

class Avatar: public Shooter
{
public:
	Avatar(StudentWorld* world, double startX, double startY):
		Shooter(world, IID_PLAYER, startX, startY, 20, right, 20)
	{}
	void heal() {setHealth(20);}
	virtual void shoot_noise();
	virtual void doSomething();
	virtual bool getHit();

};
class Robot: public Shooter
{
public:
	Robot(StudentWorld* world, int imageId, double startX, double startY, int startHealth, int dir);
	virtual void shoot_noise();
	virtual bool getHit(int score);
	bool myTick();
	bool didBasicTask(bool toShoot);
private:
	int m_tickRate;
	int m_currentTick;
};

class RageBot: public Robot
{
public:
	RageBot(StudentWorld* world, double startX, double startY, int dir);
	virtual void doSomething();
	virtual bool getHit() {return Robot::getHit(100);}
};

class ThiefBot: public Robot
{
public:
	~ThiefBot();
	ThiefBot(StudentWorld* world, double startX, double startY, bool mean = false, int bonus = 10);
	virtual bool willSteal() {return true;}
	virtual bool getHit();
	virtual void doSomething();
private:
	int m_bonus;
	bool m_mean;
	Actor* m_Pick;
	int m_distanceBeforeTurning;
	int m_currentDistance;

};

class Mean_ThiefBot: public ThiefBot
{
public:
	Mean_ThiefBot(StudentWorld* world, double startX, double startY):
		ThiefBot(world, startX, startY, true, 20)
	{}

};

class Factory: public Immutable
{
public:
	Factory(StudentWorld* world, double startX, double startY, bool mean = false):
		Immutable(world, IID_ROBOT_FACTORY, startX, startY), m_mean(mean)
	{}
	bool isMean() {return m_mean;}
	virtual void doSomething();
private:
	bool m_mean;

};


class Pea: public Actor
{
public:
	Pea(StudentWorld* world, double startX, double startY, int dir):
		Actor(world, IID_PEA, startX, startY, 2, true, dir)
	{}

	virtual void doSomething();

	void hitStatusCheck();

	virtual bool getHit() {return canBeHit();}
	virtual bool canBeHit() {return false;}


};

class Wall: public Immutable
{
public:
	Wall(StudentWorld* world, double startX, double startY):
		Immutable(world, IID_WALL, startX, startY)
	{}
};

class Marble: public Actor
{
public:
	Marble(StudentWorld* world, double startX, double startY):
		Actor(world, IID_MARBLE, startX, startY, 10)
	{}

	virtual bool pushMe(int dir);

};

class Pit: public Actor
{
public:
	Pit(StudentWorld* world, double startX, double startY):
		Actor(world, IID_PIT, startX, startY, 999)
	{}
	virtual bool getHit() {return canBeHit();}
	virtual bool canBeHit() {return false;}
	virtual bool fillMe();

};

class Pickup: public Actor
{
public:
	Pickup(StudentWorld* world, int imageId, double startX, double startY):
		Actor(world, imageId, startX, startY, 1, true) // walkable
	{}
	virtual bool getHit(){
		return canBeHit();
	}
	virtual bool canBeHit() {return false;}
	virtual void doSomething();
	virtual void giveBonus(Avatar* picker) = 0;
	virtual bool isGoodie() {return true;}
	void giveScoreAndDie(int score);
};

class Crystal: public Pickup
{
public:
	Crystal(StudentWorld* world, double startX, double startY):
		Pickup(world, IID_CRYSTAL, startX, startY)
	{}

	virtual bool isGoodie() {return false;}
	virtual void giveBonus(Avatar* picker);

};


class Life: public Pickup
{
public:
	Life(StudentWorld* world, double startX, double startY):
		Pickup(world, IID_EXTRA_LIFE, startX, startY)
	{}
	virtual void giveBonus(Avatar* picker);
};

class Health: public Pickup
{
public:
	Health(StudentWorld* world, double startX, double startY):
		Pickup(world, IID_RESTORE_HEALTH, startX, startY)
	{}
	virtual void giveBonus(Avatar* picker);
};

class Ammo: public Pickup
{
public:
	Ammo(StudentWorld* world, double startX, double startY):
		Pickup(world, IID_AMMO, startX, startY)
	{}

	virtual void giveBonus(Avatar* picker);

};

class Exit: public Pickup
{
public:
	Exit(StudentWorld* world, double startX, double startY):
		Pickup(world, IID_EXIT, startX, startY)
	{setVisible(false);}
	virtual bool revealMe();
	virtual bool canBeNeighbor();
	virtual void giveBonus(Avatar* picker);
	virtual bool getHit();
	virtual bool canBeHit();
	virtual bool isGoodie() {return false;}

private:
	bool m_revealed = false;
};

#endif // ACTOR_H_
