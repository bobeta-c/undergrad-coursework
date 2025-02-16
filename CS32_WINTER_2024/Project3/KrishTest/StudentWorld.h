#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
//#include "GraphObject.h"
#include "Actor.h"


using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
    ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  
  Level* getLev()
  {
      return lev;
  }
  Avatar* getplayerObject()
  {
      return playerObject;
  }
  bool addPea(StudentWorld* world, int col, int row, int dir)
  {
      gameObjects.push_back(new Peas(world, col, row, dir));
      return true;
  }
  bool checkAvatarMovement(int direction);
  bool removePitat(int x, int y);
  bool canrobotshoot(int direction, int x, int y);
  bool checkPeasCollide(int x, int y, int direction);
  bool canmovedirection(int x, int y, int direction);
  void assignOffsets(int direction, int& deltax, int& deltay);
  void exitdosmth();
    int getcrystalsleft()
    {
        return crystalsleft;
    }
    void crystalcollected()
    {
        crystalsleft -= 1;
    }
    bool didplayerbeatlvl()
    {
        return playerwonlvl;
    }
    void playerbeatlvl()
    {
        playerwonlvl = true;
    }
    void addLife()
    {
        incLives();
        
    }
    bool checkifongoodie(int x, int y, Actor*& goody);
    int countneighbortheifbots(int x, int y, bool& istheifbotonfactory);
    void addTheifBot(StudentWorld* world, int col, int row)
    {
        gameObjects.push_back(new TheifBots(world, col, row));
    }
    void addMeanTheifBot(StudentWorld* world, int col, int row)
    {
        gameObjects.push_back(new MeanTheifBots(world, col, row));
    }
    void dropgoody(int index)
    {
        gameObjects[index]->setVisible(true);
    }
    

    
private:
  Level* lev;
  vector<Actor*> gameObjects;
  Avatar* playerObject;
  int crystalsleft = 0;
  bool playerwonlvl = false;
    int bonus = 1000;

};

#endif // STUDENTWORLD_H_
