#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <sstream>
#include "GraphObject.h"
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), playerObject(nullptr)
{

}




int StudentWorld::init()
{
    lev = new Level(assetPath());
    ostringstream oss;
    oss.fill('0');
    oss << setw(2) << getLevel();
    string lvl = oss.str();
    
    Level::LoadResult result = lev->loadLevel("level"+lvl+".txt");
    
    if (result == Level::load_fail_file_not_found || getLevel() == 100)
        return GWSTATUS_PLAYER_WON;

    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level\n";
        for (int i = 0; i < VIEW_WIDTH; i++)
        {
            for (int j = 0; j < VIEW_HEIGHT; j++)
            {
                Level::MazeEntry ge = lev->getContentsOf(i,j); // x=5, y=10
                switch (ge)
                {
                    case Level::empty:
                        break;
                    case Level::exit:
                        gameObjects.push_back(new Exit(this, i, j));
                        break;
                    case Level::player:
                        playerObject = new Avatar(this, i, j);
                        break;
                    case Level::horiz_ragebot:
                        gameObjects.push_back(new HorizRageBot(this, i, j));
                        break;
                    case Level::vert_ragebot:
                        gameObjects.push_back(new VertRageBot(this, i, j));
                        break;
                    case Level::thiefbot_factory:
                        gameObjects.push_back(new TheifFactories(this, i, j));
                        break;
                    case Level::mean_thiefbot_factory:
                        gameObjects.push_back(new MeanTheifFactories(this, i, j));
                        break;
                    case Level::marble:
                        gameObjects.push_back(new Marbles(this, i, j));
                        break;
                    case Level::pit:
                        gameObjects.push_back(new Pits(this, i, j));
                        break;
                    case Level::crystal:
                        gameObjects.push_back(new Crystals(this, i, j));
                        crystalsleft += 1;
                        break;
                    case Level::extra_life:
                        gameObjects.push_back(new ExtraLife(this, i, j));
                        break;
                    case Level::restore_health:
                        gameObjects.push_back(new RestoreHealth(this, i, j));
                        break;
                    case Level::ammo:
                        gameObjects.push_back(new Ammo(this, i, j));
                        break;
                    case Level::wall:
                        gameObjects.push_back(new Walls(this, i, j));
                        break;
                }
            }
        }
        
    }
    return GWSTATUS_CONTINUE_GAME;
}



int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    ostringstream oss;
    oss << "Score: " << setw(7);
    oss.fill('0');
    oss << getScore();
    oss << "  Level: " << setw(2) << getLevel();
    oss.fill(' ');
    oss << "  Lives: " << setw(2) << getLives();
    double percent = (playerObject->getHealth() / 20.0) * 100;
    //oss.setf(ios::fixed);
    //oss.precision(0);
    oss << "  Health: " << setw(3) << percent << '%';
    oss << "  Ammo: " << setw(3) << playerObject->getPeas();
    oss << "  Bonus: " << setw(4) << bonus;
    
    //oss.fill('0');
    setGameStatText(oss.str());
    playerObject->doSomething();
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->doSomething();
        if (playerObject->getHealth() <= 0)
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (didplayerbeatlvl())
        {
            playerwonlvl = false;
            increaseScore(bonus);
            bonus = 1000;
            return GWSTATUS_FINISHED_LEVEL;
            
        }
    }

    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->getHealth() <= 0)
        {
            delete gameObjects[i];
            gameObjects.erase(gameObjects.begin() + i);
            i--;
        }
    }
    if (bonus >= 1)
        bonus -= 1;
    
    if (didplayerbeatlvl())
    {
        playerwonlvl = false;
        increaseScore(bonus);
        bonus = 1000;
        return GWSTATUS_FINISHED_LEVEL;
        
    }
    
    if (playerObject->getHealth() <= 0)
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
	return GWSTATUS_CONTINUE_GAME;
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}
void StudentWorld::cleanUp()
{
    if (playerObject != nullptr)
    {
        delete playerObject;
        playerObject = nullptr;
    }
    
    for (int i = 0; i < gameObjects.size(); i++)
        delete gameObjects[i];
    gameObjects.clear();
}

bool StudentWorld::checkAvatarMovement(int direction)
{
    int deltaX = 0;
    int deltaY = 0;
    assignOffsets(direction, deltaX, deltaY);

    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->getX() == playerObject->getX() + deltaX && gameObjects[i]->getY() == playerObject->getY() + deltaY)
        {
            if ((gameObjects[i]->canstopmove() && !gameObjects[i]->canbemoved())|| gameObjects[i]->canbefilled())
                return false;
        }
    }

    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->getX() == playerObject->getX() + deltaX && gameObjects[i]->getY() == playerObject->getY() + deltaY && gameObjects[i]->canbemoved())
        {
            for (int j = 0; j < gameObjects.size(); j++)
            {
                if (!gameObjects[j]->canbefilled() && !gameObjects[j]->canbepickedup() && gameObjects[j]->isVisible() && gameObjects[j]->getX() == playerObject->getX() + deltaX * 2 && gameObjects[j]->getY() == playerObject->getY() + deltaY * 2)
                    return false;
                if (playerObject->getX() + deltaX * 2 ==  gameObjects[j]->getX() + deltaX && playerObject->getY() + deltaY * 2 ==  gameObjects[j]->getY() + deltaY && getLev()->getContentsOf(gameObjects[j]->getX() + deltaX, gameObjects[j]->getY() + deltaY) == Level::exit)
                    return false;
                    
                if (gameObjects[j]->canbepickedup() && gameObjects[j]->isVisible() && gameObjects[j]->getX() == playerObject->getX() + deltaX * 2 && gameObjects[j]->getY() == playerObject->getY() + deltaY * 2)
                    return false;
            }
            gameObjects[i]->moveTo(playerObject->getX() + deltaX * 2, playerObject->getY() + deltaY * 2);
            playerObject->moveTo(playerObject->getX() + deltaX, playerObject->getY() + deltaY);
            return true;
        }
    }

    playerObject->moveTo(playerObject->getX() + deltaX, playerObject->getY() + deltaY);
    return true;
}

bool StudentWorld::removePitat(int x, int y)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->canbemoved() && gameObjects[i]->getX() == x && gameObjects[i]->getY() == y)
        {
            gameObjects[i]->damage(10);
            return true;
        }
    }
    return false;
}

bool StudentWorld::canrobotshoot(int direction, int x, int y)
{
    if (direction == Actor::right)
    {
        if (getplayerObject()->getY() == y && getplayerObject()->getX() > x)
        {
            for (int i = 0; i < gameObjects.size(); i++)
            {
                if ((gameObjects[i]->canstopmove() && !gameObjects[i]->canbefilled())|| gameObjects[i]->canbemoved())
                {
                    if (gameObjects[i]->getX() < getplayerObject()->getX() && gameObjects[i]->getX() > x && gameObjects[i]->getY() == y)
                    {
                        return false;
                    }
                }
            }
        }
        else
            return false;
    }
    else if (direction == Actor::left)
    {
        if (getplayerObject()->getY() == y && getplayerObject()->getX() < x)
        {
            for (int i = 0; i < gameObjects.size(); i++)
            {
                if ((gameObjects[i]->canstopmove() && !gameObjects[i]->canbefilled())|| gameObjects[i]->canbemoved())
                {
                    if (gameObjects[i]->getX() > getplayerObject()->getX() && gameObjects[i]->getX() < x && gameObjects[i]->getY() == y)
                    {
                        return false;
                    }
                }
            }
        }
        else
            return false;
    }
    else if (direction == Actor::up)
    {
        if (getplayerObject()->getY() > y && getplayerObject()->getX() == x)
        {
            for (int i = 0; i < gameObjects.size(); i++)
            {
                if ((gameObjects[i]->canstopmove() && !gameObjects[i]->canbefilled())|| gameObjects[i]->canbemoved())
                {
                    if (gameObjects[i]->getY() < getplayerObject()->getY() && gameObjects[i]->getY() > y && gameObjects[i]->getX() == x)
                    {
                        return false;
                    }
                }
            }
        }
        else
            return false;
    }
    else if (direction == Actor::down)
    {
        if (getplayerObject()->getY() < y && getplayerObject()->getX() == x)
        {
            for (int i = 0; i < gameObjects.size(); i++)
            {
                if ((gameObjects[i]->canstopmove() && !gameObjects[i]->canbefilled())|| gameObjects[i]->canbemoved())
                {
                    if (gameObjects[i]->getY() > getplayerObject()->getY() && gameObjects[i]->getY() < y && gameObjects[i]->getX() == x)
                    {
                        return false;
                    }
                }
            }
        }
        else
            return false;
    }
    return true;
}

void StudentWorld::assignOffsets(int direction, int& deltaX, int& deltaY)
{
    if (direction == Actor::right)
    {
        deltaX = 1;
    }
    else if (direction == Actor::left)
    {
        deltaX = -1;
    }
    else if (direction == Actor::up)
    {
        deltaY = 1;
    }
    else if (direction == Actor::down)
    {
        deltaY = -1;
    }
}
bool StudentWorld::canmovedirection(int x, int y, int direction)
{
    int deltaX = 0;
    int deltaY = 0;
    assignOffsets(direction, deltaX, deltaY);

    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->canstopmove())
        {
            if (gameObjects[i]->getX() == x + deltaX && gameObjects[i]->getY() == y + deltaY)
            {
                return false;
            }
            if (playerObject->getX() == x + deltaX && playerObject->getY() == y + deltaY)
                return false;
        }
    }
    return true;
}

bool StudentWorld::checkPeasCollide(int x, int y, int direction)
{
    int deltax = 0;
    int deltay = 0;
    assignOffsets(direction, deltax, deltay);
    
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (playerObject->canbedamagedbypea() && playerObject->getX() == x && playerObject->getY() == y)
        {
            playerObject->damage(2);
            if (playerObject->getHealth() > 0)
                playSound(SOUND_PLAYER_IMPACT);
            else
                playSound(SOUND_PLAYER_DIE);
            return false;
        }
        
        if (gameObjects[i]->canbedamagedbypea() && gameObjects[i]->getX() == x && gameObjects[i]->getY() == y)
        {
            gameObjects[i]->damage(2);
            return false;
        }
       
        if (getLev()->getContentsOf(x, y) == Level::wall || getLev()->getContentsOf(x, y) == Level::thiefbot_factory || getLev()->getContentsOf(x, y) == Level::mean_thiefbot_factory)
        {
            return false;
        }
         
    }
     
    return true;
}

bool StudentWorld::checkifongoodie(int x, int y, Actor*& goody)
{
    int rand = randInt(1, 10);
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->canbepickedup() && gameObjects[i]->isVisible())
        {
            if (rand == 1 && gameObjects[i]->getX() == x && gameObjects[i]->getY() == y)
            {
                goody = gameObjects[i];
                gameObjects[i]->setVisible(false);
                return true;
            
            }
        }
    }
    return false;
}

int StudentWorld::countneighbortheifbots(int x, int y, bool& istheifbotonfactory)
{
    int count = 0;
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->cansteal() && gameObjects[i]->getX() <= x + 3 && gameObjects[i]->getX() >= x - 3 && gameObjects[i]->getY() <= y + 3 && gameObjects[i]->getY() >= y - 3)
            count += 1;
        if (gameObjects[i]->cansteal() && gameObjects[i]->getX() == x && gameObjects[i]->getY() == y)
            istheifbotonfactory = true;
    }
    return count;
}
