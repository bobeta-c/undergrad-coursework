#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  ~StudentWorld();
  bool moveActor(Actor* a, int dir, bool canPush = false);
  bool pushMarble(Marble* m, int dir);
  bool pea_status_check(Pea* pea);
  bool gen_status_check(int x, int y, Actor* toBeIgnored = nullptr);
  void generate_pea(int x, int y, int dir);
  void generate_thief(int x, int y, bool mean);
  void generate_actor(int x, int y, Actor* a);
  int getBonus() {return m_bonus;}
  bool avatarInSight(Shooter* s, int startX = -1, int startY = -1);
  void avatar_pickup(Pickup* p);
  void killCrystal() {m_crystals--;}
  void addScore(int amount) {m_score += amount; m_totalScore += amount;}
  void makeItWin() {m_level_complete = true;}
  bool canFactorySpawn(Factory* f);
  Actor* thiefSomething(ThiefBot* thief);

private:
  void updateDisplayText();
  void removeDeadGameObjects();
  void reduceLevelBonusByOne();
  void exposeExit();
  bool noMoreCrystals();
  bool isLevelComplete();
  std::vector<Actor*> actorsAt(int x, int y);
  
  bool m_level_complete;
  int m_bonus;
  int m_crystals;
  int m_score;
  std::vector<Actor*> m_Actors;
  Avatar* m_Avatar;
  int m_totalScore;
};

void updateCoords(int& x, int& y, int dir);


#endif // STUDENTWORLD_H_
