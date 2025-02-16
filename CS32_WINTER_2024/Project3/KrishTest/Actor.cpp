#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Avatar::move(int direction)
{
    setDirection(direction);
    getWorld()->checkAvatarMovement(direction);
    
}


void Avatar::doSomething()
{
    if (getHealth() > 0)
    {
        int ch;
        if (getWorld()->getKey(ch))
        {
            switch (ch)
            {
                case KEY_PRESS_SPACE:
                {
                    int dir = getDirection();
                    if (getWorld()->getplayerObject()->usePea())
                    {
                        int deltax = 0;
                        int deltay = 0;
                        getWorld()->assignOffsets(dir, deltax, deltay);
                        getWorld()->addPea(getWorld(), getX() , getY(), dir);
                        getWorld()->playSound(SOUND_PLAYER_FIRE);
                    }
                         
                    break;
                }
                case KEY_PRESS_LEFT:
                    move(left);
                    break;
                case KEY_PRESS_RIGHT:
                    move(right);
                    break;
                case KEY_PRESS_UP:
                    move(up);
                    break;
                case KEY_PRESS_DOWN:
                    move(down);
                    break;
                case KEY_PRESS_ESCAPE:
                    damage(1000);
            }
        }
    }
    else
        return;
}

bool Peas::checkCollide()
{
    if (!(getWorld()->checkPeasCollide(getX(), getY(), getDirection())))
    {
        setVisible(false);
        damage(20);
        return false;
    }
    return true;
    
}

void Crystals::doSomething()
{
    if (getHealth() <= 0)
        return;
    if (getWorld()->getplayerObject()->getX() == getX() && getWorld()->getplayerObject()->getY() == getY())
    {
        getWorld()->increaseScore(50);
        damage(20);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->crystalcollected();
        
    }

}

void Peas::doSomething()
{
    if (getHealth() <= 0)
        return;
    int currx = getX();
    int curry = getY();
    int dir = getDirection();
    if (dir == right)
        moveTo(currx + 1, curry);
    else if (dir == left)
        moveTo(currx - 1, curry);
    else if (dir == up)
        moveTo(currx, curry + 1);
    else if (dir == down)
        moveTo(currx, curry - 1);
    checkCollide();
}

void RageBots::doSomething()
{
    if (getHealth() <= 0)
        return;
    setTickDivider((28 - getWorld()->getLevel()) / 4);
    if (gettickdivider() < 3)
        setTickDivider(3);
    
    if (getTicks() % gettickdivider() == 0 && getTicks() > 0)
    {
        if (getWorld()->canrobotshoot(getDirection(), getX(), getY()))
        {
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            getWorld()->addPea(getWorld(), getX(), getY(), getDirection());
        }
        else
        {
            if (getDirection() == right)
            {
                if (getWorld()->canmovedirection(getX(), getY(), right))
                    moveTo(getX() + 1, getY());
                else
                    setDirection(left);
            }
            else if (getDirection() == left)
            {
                if (getWorld()->canmovedirection(getX(), getY(), left))
                    moveTo(getX() - 1, getY());
                else
                    setDirection(right);
            }
            else if (getDirection() == up)
            {
                if (getWorld()->canmovedirection(getX(), getY(), up))
                    moveTo(getX(), getY()+1);
                else
                    setDirection(down);
            }
            else if (getDirection() == down)
            {
                if (getWorld()->canmovedirection(getX(), getY(), down))
                    moveTo(getX(), getY()-1);
                else
                    setDirection(up);
            }
        }
    }
         
             
    incrementTicks();
     
}

void Pits::doSomething()
{
    if (getHealth() <= 0)
        return;
    if (getWorld()->removePitat(getX(), getY()))
        damage(20);
}

void Exit::doSomething()
{
    int deltax = 0;
    int deltay = 0;
    getWorld()->assignOffsets(getWorld()->getplayerObject()->getDirection(), deltax, deltay);
    if (getWorld()->getcrystalsleft() == 0 && getWorld()->getplayerObject() != nullptr)
    {
        setVisible(true);
        if (!playedsound)
        {
            getWorld()->playSound(SOUND_REVEAL_EXIT);
            playedsound = true;
            
        }
    }
    if (isVisible() && getWorld()->getplayerObject()->getX() == getX() && getWorld()->getplayerObject()->getY() == getY())
    {
        getWorld()->increaseScore(2000);
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->playerbeatlvl();
    }
    
    
}

void ExtraLife::doSomething()
{
    if (getHealth() <= 0)
        return;
    
    if (getWorld()->getplayerObject()->getX() == getX() && getWorld()->getplayerObject()->getY() == getY() && isVisible())
    {
        getWorld()->increaseScore(1000);
        damage(20);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->addLife();
        
    }
    
}

void RestoreHealth::doSomething()
{
    if (getHealth() <= 0)
        return;
    
    if (getWorld()->getplayerObject()->getX() == getX() && getWorld()->getplayerObject()->getY() == getY() && isVisible())
    {
        getWorld()->increaseScore(500);
        damage(20);
        getWorld()->getplayerObject()->restoreHealth();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    
}

void Ammo::doSomething()
{
    if (getHealth() <= 0)
        return;
    
    if (getWorld()->getplayerObject()->getX() == getX() && getWorld()->getplayerObject()->getY() == getY() && isVisible())
    {
        getWorld()->increaseScore(100);
        damage(20);
        getWorld()->getplayerObject()->addPeas();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    
}

void Theifs::doSomething()
{
    
    if (getHealth() <= 0)
        return;
    setTickDivider((28 - getWorld()->getLevel()) / 4);
    if (gettickdivider() < 3)
         setTickDivider(3);
    
    if (getTicks() % gettickdivider() == 0 && getTicks() > 0)
    {
        int deltaX = 0;
        int deltaY = 0;
        getWorld()->assignOffsets(getDirection(), deltaX, deltaY);
        
        if (canshoot() && getWorld()->canrobotshoot(getDirection(), getX(), getY()))
        {
            getWorld()->playSound(SOUND_ENEMY_FIRE);
            getWorld()->addPea(getWorld(), getX(), getY(), getDirection());
        }
        
        else if (!getpickedupgoody() && getWorld()->checkifongoodie(getX(), getY(), goody))
        {
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            setpickedupgoody();
        }
        else
        {
            if (getdistancemoved() < getdistancebeforeturning() && getWorld()->canmovedirection(getX(), getY(), getDirection()))
            {
                moveTo(getX() + deltaX, getY() + deltaY);
                incdistancemoved();
            }
            else
            {
                setdistancebeforeturning(randInt(1, 6));
                int dir = randInt(1, 4);
                if (dir == 1)
                {
                    setDirection(right);
                    if (getWorld()->canmovedirection(getX(), getY(), right))
                    {
                        moveTo(getX() + 1, getY());
                        setmoved();
                    }
                }
                else if (dir == 2)
                {
                    setDirection(left);
                    if (getWorld()->canmovedirection(getX(), getY(), left))
                    {
                        moveTo(getX() - 1, getY());
                        setmoved();
                    }
                }
                else if (dir == 3)
                {
                    setDirection(up);
                    if (getWorld()->canmovedirection(getX(), getY(), up))
                    {
                        moveTo(getX(), getY() + 1);
                        setmoved();
                    }
                    
                }
                else
                {
                    setDirection(down);
                    if (getWorld()->canmovedirection(getX(), getY(), down))
                    {
                        moveTo(getX(), getY() - 1);
                        setmoved();
                    }
                    
                }
                setdistancemoved(1);

                while (!getWorld()->canmovedirection(getX(), getY(), getDirection()) && !getmoved())
                {
                    if (!checktrieddir(right) && getDirection() != right && getWorld()->canmovedirection(getX(), getY(), right))
                    {
                        moveTo(getX() + 1, getY());
                        setDirection(right);
                        trieddir(right);
                        break;
                    }
                    else if (!checktrieddir(left) && getDirection() != left && getWorld()->canmovedirection(getX(), getY(), left))
                    {
                        moveTo(getX() - 1, getY());
                        setDirection(left);
                        trieddir(left);
                        break;
                    }
                    else if (!checktrieddir(up) && getDirection() != up && getWorld()->canmovedirection(getX(), getY(), up))
                    {
                        moveTo(getX(), getY() + 1);
                        setDirection(up);
                        trieddir(up);
                        break;
                    }
                    else if (!checktrieddir(down) && getDirection() != down && getWorld()->canmovedirection(getX(), getY(), down))
                    {
                        moveTo(getX(), getY() - 1);
                        setDirection(down);
                        trieddir(down);
                        break;
                    }
                    else if ((checktrieddir(right) && checktrieddir(left) && checktrieddir(down) && checktrieddir(up)) || (!getWorld()->canmovedirection(getX(), getY(), left) &&
                                                                                   !getWorld()->canmovedirection(getX(), getY(), right) && !getWorld()->canmovedirection(getX(), getY(), down) && !getWorld()->canmovedirection(getX(), getY(), up)))
                        break;
                }
                
            }
        }
        resetbools();
    }
         
             
    incrementTicks();
     
}

void TheifFactories::doSomething()
{
    bool istheifbotonfactory = false;
    if (getWorld()->countneighbortheifbots(getX(), getY(), istheifbotonfactory) < 3 && !istheifbotonfactory)
    {
        int rand = randInt(1, 50);
        if (rand == 1)
        {
            if (makesRegularTheifs())
                getWorld()->addTheifBot(getWorld(), getX(), getY());
            else
                getWorld()->addMeanTheifBot(getWorld(), getX(), getY());
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}

bool Theifs::damage(int num)
{
    Actor::damage(num);
    if (getHealth() <= 0)
    {
        if (goody != nullptr)
        {
            goody->moveTo(getX(), getY());
            goody->setVisible(true);
            getWorld()->playSound(SOUND_ROBOT_DIE);
            if (canshoot())
                getWorld()->increaseScore(20);
            else
                getWorld()->increaseScore(10);
        }
        else
        {
            if (canshoot())
            {
                getWorld()->playSound(SOUND_ROBOT_DIE);
                getWorld()->increaseScore(20);
            }
            else
            {
                getWorld()->playSound(SOUND_ROBOT_DIE);
                getWorld()->increaseScore(10);
            }
                
        }
    }
    else
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    return true;
}

bool RageBots::damage(int num)
{
    Actor::damage(num);
    if (getHealth() <= 0)
        getWorld()->playSound(SOUND_ROBOT_DIE);
    else
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
        getWorld()->increaseScore(100);
    }
    return true;
}
