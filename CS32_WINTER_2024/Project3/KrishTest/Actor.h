#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
using namespace std;
#include "GameWorld.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int col, int row, int health) : GraphObject(imageID, col, row), m_world(world), m_health(health) {}
    StudentWorld* getWorld() const
    {
        return m_world;
    }
    virtual void doSomething() = 0;
    
    int getHealth()
    {
        return m_health;
    }
    virtual bool damage(int num)
    {
        m_health -= num;
        return true;
    }
    virtual bool canbedamagedbypea()
    {
        return false;
    }
    virtual bool canstopmove()
    {
        return true;
    }
    virtual int canbemoved()
    {
        return false;
    }
    virtual int canbefilled()
    {
        return false;
    }
    virtual bool canbepickedup()
    {
        return false;
    }
    void restoreHealth()
    {
        m_health = 20;
    }
    virtual bool cansteal()
    {
        return false;
    }
    virtual bool canshoot()
    {
        return false;
    }
    
private:
    StudentWorld* m_world = nullptr;
    int m_health;
    
};

class Avatar : public Actor
{
public:
    Avatar(StudentWorld* world, int col, int row) : Actor(world, IID_PLAYER, col, row, 20), peas(20)
    {
        setVisible(true);
        setDirection(right);
    }
    virtual void doSomething();
    void addPeas()
    {
        peas += 20;
    }
    virtual bool canbedamagedbypea()
    {
        return true;
    }
    virtual bool canshoot()
    {
        return true;
    }
    int getPeas()
    {
        return peas;
    }
    
private:
    void checkMoveMarble(int dir);
    void move(int direction);
    bool usePea()
    {
        if (peas > 0)
        {
            peas -= 1;
            return true;
        }
        return false;
    }
    int peas;
};

class Walls : public Actor
{
public:
    Walls(StudentWorld* world, int col, int row) : Actor(world, IID_WALL, col, row, 100)
    {
        setVisible(true);
        setDirection(none);
    }
    virtual void doSomething() {}
};

class Marbles : public Actor
{
public:
    Marbles(StudentWorld* world, int col, int row) : Actor(world, IID_MARBLE, col, row, 10)
    {
        setVisible(true);
        setDirection(none);
    }
    virtual void doSomething() {}
    virtual bool canbedamagedbypea()
    {
        return true;
    }
    virtual int canbemoved()
    {
        return true;
    }
};



class Peas : public Actor
{
public:
    Peas(StudentWorld* world, int col, int row, int dir) : Actor(world, IID_PEA, col, row, 20)
    {
        setVisible(true);
        setDirection(dir);
    }
    bool checkCollide();
    virtual void doSomething();
    virtual bool canstopmove()
    {
        return false;
    }
};

class Pits : public Actor
{
public:
    Pits(StudentWorld* world, int col, int row) : Actor(world, IID_PIT, col, row, 20)
    {
        setVisible(true);
        setDirection(none);
    }
    virtual void doSomething();
    virtual int canbefilled()
    {
        return true;
    }
};

class Crystals : public Actor
{
public:
    Crystals(StudentWorld* world, int col, int row) : Actor(world, IID_CRYSTAL, col, row, 20)
    {
        setVisible(true);
        setDirection(none);
    }
    virtual void doSomething();
    virtual bool canstopmove()
    {
        return false;
    }
};

class Exit : public Actor
{
public:
    Exit(StudentWorld* world, int col, int row) : Actor(world, IID_EXIT, col, row, 20)
    {
        setVisible(false);
        setDirection(none);
    }
    virtual void doSomething();
    virtual bool canstopmove()
    {
        return false;
    }

private:
    bool playedsound = false;
};

class Bot: public Actor
{
public:
    Bot(StudentWorld* world, int ID, int col, int row, int dir, int hp): Actor(world, ID, col, row, hp)
    {
        setDirection(dir);
        setVisible(true);
    }
    int getTicks()
    {
        return ticks;
    }
    void incrementTicks()
    {
        ticks += 1;
    }
    void setTickDivider(int divider)
    {
        tickdivider = divider;
    }
    int gettickdivider()
    {
        return tickdivider;
    }
private:
    int ticks = 0;
    int tickdivider;
};

class RageBots : public Bot
{
public:
    RageBots(StudentWorld* world, int col, int row, int dir) : Bot(world, IID_RAGEBOT, col, row, dir, 10) {}
    virtual void doSomething();
    virtual bool canbedamagedbypea()
    {
        return true;
    }
    virtual bool canshoot()
    {
        return true;
    }
    virtual bool damage(int num);
    
};

class HorizRageBot: public RageBots
{
public:
    HorizRageBot(StudentWorld *world, int col, int row) : RageBots(world, col, row, right) {}
};

class VertRageBot: public RageBots
{
public:
    VertRageBot(StudentWorld *world, int col, int row) : RageBots(world, col, row, up) {}
};

class ExtraLife : public Actor
{
public:
    ExtraLife(StudentWorld* world, int col, int row) : Actor(world, IID_EXTRA_LIFE, col, row, 10) 
    {
        setVisible(true);
        setDirection(none);
    }
    virtual void doSomething();
    virtual bool canstopmove()
    {
        return false;
    }
    virtual bool canbepickedup()
    {
        return true;
    }
};

class RestoreHealth : public Actor
{
public:
    RestoreHealth(StudentWorld* world, int col, int row) : Actor(world, IID_RESTORE_HEALTH, col, row, 10)
    {
        setVisible(true);
        setDirection(none);
    }
    virtual void doSomething();
    virtual bool canstopmove()
    {
        return false;
    }
    virtual bool canbepickedup()
    {
        return true;
    }
};

class Ammo : public Actor
{
public:
    Ammo(StudentWorld* world, int col, int row) : Actor(world, IID_AMMO, col, row, 10)
    {
        setVisible(true);
        setDirection(none);
    }
    virtual void doSomething();
    virtual bool canstopmove()
    {
        return false;
    }
    virtual bool canbepickedup()
    {
        return true;
    }
};

class Theifs : public Bot
{
public:
    Theifs(StudentWorld* world, int ID, int col, int row, int hp) : Bot(world, ID, col, row, right, hp)
    {
        distanceBeforeTurning = randInt(1, 6);
    }
    virtual bool canbedamagedbypea()
    {
        return true;
    }
    virtual bool cansteal()
    {
        return true;
    }
    void incdistancemoved()
    {
        distancemoved += 1;
    }
    int getdistancemoved()
    {
        return distancemoved;
    }
    void setdistancemoved(int distance)
    {
        distancemoved = distance;
    }
    void setdistancebeforeturning(int distance)
    {
        distanceBeforeTurning = distance;
    }
    int getdistancebeforeturning()
    {
        return distanceBeforeTurning;
    }
    void trieddir(int dir)
    {
        if (dir == right)
            triedright = true;
        else if (dir == left)
            triedleft = true;
        else if (dir == up)
            triedup = true;
        else
            trieddown = true;
    }
    bool checktrieddir(int dir)
    {
        if (dir == right && triedright) return true;
        if (dir == left && triedleft) return true;
        if (dir == up && triedup) return true;
        if (dir == down && trieddown) return true;
        return false;
    }
    void resetbools()
    {
        triedup = false;
        trieddown = false;
        triedleft = false;
        triedright = false;
        moved = false;
    }
    void setmoved()
    {
        moved = true;
    }
    bool getmoved()
    {
        return moved;
    }
    void setpickedupgoody()
    {
        pickedupgoody = true;
    }
    bool getpickedupgoody()
    {
        return pickedupgoody;
    }
    virtual void doSomething();
    virtual bool damage(int num);

private:
    int distanceBeforeTurning;
    int distancemoved = 0;
    bool triedright = false;
    bool triedleft = false;
    bool triedup = false;
    bool trieddown = false;
    bool pickedupgoody = false;
    bool moved = false;
    Actor* goody = nullptr;
};

class TheifBots : public Theifs
{
public:
    TheifBots(StudentWorld* world, int col, int row) : Theifs(world, IID_THIEFBOT, col, row, 5) {}
};

class MeanTheifBots : public Theifs
{
public:
    MeanTheifBots(StudentWorld* world, int col, int row) : Theifs(world, IID_MEAN_THIEFBOT, col, row, 8) {}
    virtual bool canshoot()
    {
        return true;
    }
};

class TheifFactories : public Actor
{
public:
    TheifFactories(StudentWorld* world, int col, int row) : Actor(world, IID_ROBOT_FACTORY, col, row, 5)
    {
        setVisible(true);
        setDirection(none);
        
    }
    virtual bool makesRegularTheifs()
    {
        return true;
    }
    virtual void doSomething();
};

class MeanTheifFactories : public TheifFactories
{
public:
    MeanTheifFactories(StudentWorld* world, int col, int row) : TheifFactories(world, col, row) {}
    virtual bool makesRegularTheifs()
    {
        return false;
    }
    
};

#endif // ACTOR_H_
