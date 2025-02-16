#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Level.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_crystals(0), m_score(0), m_totalScore(0), m_bonus(0), m_level_complete(false)
{
}

StudentWorld::~StudentWorld()
{cleanUp();}

int StudentWorld::init()
{
	Level lev(assetPath());

	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << getLevel() << ".txt"; //get the level postfix
	Level::LoadResult result = lev.loadLevel(oss.str());
	m_bonus = 1000;
	m_level_complete = false;
	m_crystals = 0;
	if (result == Level::load_fail_file_not_found ||
	 result == Level:: load_fail_bad_format)
	 return -1;
	
	Level::MazeEntry item;

	for(int x = 0; x < VIEW_WIDTH; x++){
		for(int y = 0; y < VIEW_HEIGHT; y++){
			item = lev.getContentsOf(x, y);
			switch (item)
			{
			case Level::empty:
				break;
			case Level::exit:
				m_Actors.push_back(new Exit(this, x, y));
				break;
			case Level::player:
				m_Avatar = new Avatar(this, x, y);
				break;
			case Level::horiz_ragebot:
				m_Actors.push_back(new RageBot(this, x, y, Actor::right));
				break;
			case Level::vert_ragebot:
				m_Actors.push_back(new RageBot(this, x, y, Actor::up));
				break;
			case Level::thiefbot_factory:
				m_Actors.push_back(new Factory(this, x, y, false));
				break;
			case Level::mean_thiefbot_factory:
				m_Actors.push_back(new Factory(this, x, y, true));
				break;
			case Level::marble:
				m_Actors.push_back(new Marble(this, x, y));
				break;
			case Level::pit:
				m_Actors.push_back(new Pit(this, x, y));
				break;
			case Level::crystal:
				m_crystals++;
				m_Actors.push_back(new Crystal(this, x, y));
				break;
			case Level::restore_health:
				m_Actors.push_back(new Health(this, x, y));
				break;
			case Level::extra_life:
				m_Actors.push_back(new Life(this, x, y));
				break;
			case Level::ammo:
				m_Actors.push_back(new Ammo(this, x, y));
				break;
			case Level::wall:
				m_Actors.push_back(new Wall(this, x, y));
				break;
			}
		}
	}
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::generate_actor(int x, int y, Actor* a){
	a->moveTo(x, y);
	m_Actors.push_back(a);
	a->setVisible(true);
}

Actor* StudentWorld::thiefSomething(ThiefBot* thief)
{
	Actor* returnThis;
	vector<Actor*> p = actorsAt(thief->getX(), thief->getY());
	for(vector<Actor*>::iterator q = p.begin(); q != p.end(); q++){
		if((*q)->isGoodie()){
			returnThis = (*q);
			for(vector<Actor*>::iterator l = m_Actors.begin(); ; l++){
				if((*l) == (*q)){
					(*q)->setVisible(false);
					m_Actors.erase(l);
					break;
				}
				if(l == m_Actors.end()){
					cout << "ABORT" << endl;
					break;
				}
			}
			return returnThis;
		}
	}
	return nullptr;
}

int StudentWorld::move()
{
	updateDisplayText(); // update the score/lives/level text at screen top


	for(int i = 0; i < m_Actors.size(); i++){
		Actor* p = m_Actors[i];
    	if(p->isAlive())
    		p->doSomething();
    	if (!(m_Avatar->isAlive()))
    	{
    		decLives();
    		return GWSTATUS_PLAYER_DIED;
    	}
    }
    m_Avatar->doSomething();
    //TODO (check if player has completed the current level)

    removeDeadGameObjects(); // delete dead game objects

    // Reduce the current bonus for the Level by one
	reduceLevelBonusByOne(); 

	// If the player has collected all of the crystals on the level, then we
	// must expose the exit so the player can advance to the next level
	if (noMoreCrystals())
		exposeExit(); // make the exit Active
	// return the proper result
	if (!(m_Avatar->isAlive()))
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	if (isLevelComplete())
	{
		//INCREASE SCORE
		return GWSTATUS_FINISHED_LEVEL;
	}
	// the player hasn’t completed the current level and hasn’t died, so
	// continue playing the current level
	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::canFactorySpawn(Factory* f)
{
	if(gen_status_check(f->getX(), f->getY(), f)){
		return false;
	}
	int count = 0;
	for(int x = f->getX() - 3; x <= f->getX() + 3; x++){
		for(int y = f->getY() - 3; y <= f->getY() + 3; y++){
			vector<Actor*> potentialRobots = actorsAt(x, y);
			for(vector<Actor*>::iterator p = potentialRobots.begin(); p != potentialRobots.end(); p++){
				if((*p)->willSteal()){
					count++;
				}
			}
		}

	}
	return count < 3;
}


void StudentWorld::cleanUp()
{
	increaseScore(m_score);
	m_score = 0;
	for(vector<Actor*>::iterator p = m_Actors.begin(); p != m_Actors.end(); p++){
		delete (*p);
	}
	delete m_Avatar;
	m_Avatar = nullptr;
	m_Actors.clear();
}

bool StudentWorld::isLevelComplete()
{
	return m_level_complete;
}

bool StudentWorld::noMoreCrystals()
{

	return (m_crystals == 0);
}	

void StudentWorld::exposeExit()
{
	for(vector<Actor*>::iterator p = m_Actors.begin(); p != m_Actors.end(); p++)
	{
		(*p)->revealMe();
	}
}

void StudentWorld::reduceLevelBonusByOne()
{
	if(m_bonus != 0)
		m_bonus--;
}

void StudentWorld::removeDeadGameObjects()
{
	for(vector<Actor*>::iterator p = m_Actors.begin(); p != m_Actors.end();)
	{
		if (!(*p)->isAlive()){
			delete(*p);
			p = m_Actors.erase(p);
		}
		else p++;
	}
}

void StudentWorld::updateDisplayText()
{
	int score = m_totalScore;
	int level = getLevel();
	unsigned int bonus = m_bonus;
	int livesLeft = getLives();
	ostringstream oss;
	oss.fill('0');
	oss << "Score: " << setw(7) << score << "  Level: " << setw(2) << level << "  Lives: " 
		<< setfill(' ') << setw(2) << livesLeft << "  Health: " << setw(3) << 100*(m_Avatar->getHealth() + 0.0)/20
		<< "%  Ammo: " << setw(3) << m_Avatar->getPeas() << "  Bonus: " << setw(4) << bonus; 



	setGameStatText(oss.str());
}

vector<Actor*> StudentWorld::actorsAt(int x, int y)
{
	vector<Actor*> returnActors;
	for(vector<Actor*>::iterator p = m_Actors.begin(); p != m_Actors.end(); p++)
	{
		if ((*p)->getX() == x && (*p)->getY() == y){
			returnActors.push_back(*p);
		}
	}
	return returnActors;
}

void StudentWorld::avatar_pickup(Pickup* p){
	if(m_Avatar->isAlive()){
		if(m_Avatar->getX() == p->getX() && m_Avatar->getY() == p->getY())
			p->giveBonus(m_Avatar);
	}
}


void updateCoords(int& x, int& y, int dir){
	switch (dir)
		{
		case Actor::up:
			y++;
			break;
		case Actor::down:
			y--;
			break;
		case Actor::left:
			x--;
			break;
		case Actor::right:
			x++;
			break;
		default:
			cout << "ABORT2" << endl;
		}
}


bool StudentWorld::avatarInSight(Shooter* s, int startX, int startY){
	if(startX == -1 || startY == -1){
		startX = s->getX();
		startY = s->getY();
	}
	updateCoords(startX, startY, s->getDirection());
	if(m_Avatar->getX() == startX && m_Avatar->getY() == startY)
		return true;
	if(gen_status_check(startX, startY)){
		return false;
	}
	return avatarInSight(s, startX, startY);

}

bool StudentWorld::moveActor(Actor* a, int dir, bool canPush){
	a->setDirection(dir);
	int x = a->getX();
	int y = a->getY();
	updateCoords(x,y,dir);
	vector<Actor*> futureSquare = actorsAt(x, y);
	if(m_Avatar->getX() == x && m_Avatar->getY() == y)
		futureSquare.push_back(m_Avatar);
	if(!a->canBeNeighbor()){
		for(vector<Actor*>::iterator p = futureSquare.begin(); p != futureSquare.end(); p++)
		{
			if(canPush && (*p)->pushMe(dir)){
				continue;
			}
			if((*p != a) && !((*p)->canBeNeighbor())){
				return false;
			}
		}
	}
	a->moveTo(x, y);
	return true;
}

bool StudentWorld::pushMarble(Marble* m, int dir){
	int x = m->getX();
	int y = m->getY();
	updateCoords(x,y,dir);
	vector<Actor*> futureSquare = actorsAt(x,y);
	for(vector<Actor*>::iterator p = futureSquare.begin(); p != futureSquare.end(); p++){
		if((*p) != m && (*p)->fillMe()){
			m->setHealth(0);
			return true;
		}
		return false;
	}
	m->moveTo(x,y);
	return true;
}
void StudentWorld::generate_thief(int x, int y, bool mean){
	if(mean){
		m_Actors.push_back(new Mean_ThiefBot(this, x, y));
		return;
	}
	m_Actors.push_back(new ThiefBot(this, x, y));
}

void StudentWorld::generate_pea(int x, int y, int dir){
	m_Actors.push_back(new Pea(this, x, y, dir));
	vector<Actor*>::iterator p = m_Actors.end();
	p--;
	moveActor(*p, dir);
}

bool StudentWorld::gen_status_check(int x, int y, Actor* toBeIgnored){
	vector<Actor*> on_square = actorsAt(x, y);
	if(m_Avatar->getX() == x && m_Avatar->getY() == y)
		on_square.push_back(m_Avatar);
	bool something_here = false;
	for(vector<Actor*>::iterator p = on_square.begin(); p != on_square.end(); p++)
	{
		if((*p) != toBeIgnored && (*p)->canBeHit())
			something_here = true;

	}
	return something_here;
}
bool StudentWorld::pea_status_check(Pea* pea){
	vector<Actor*> with_pea = actorsAt(pea->getX(), pea->getY());
	if(m_Avatar->getX() == pea->getX() && m_Avatar->getY() == pea->getY())
		with_pea.push_back(m_Avatar);
	bool hit_something = false;
	for(vector<Actor*>::iterator p = with_pea.begin(); p != with_pea.end(); p++)
	{
		if((*p != pea))
			if((*p)->getHit())
				hit_something = true;
	}
	return hit_something;
}
